#include "Demo.h"
#include "Window.h"
#include "DrawBatch.h"
#include "Input.h"
#include "Sprite.h"
#include "Stopwatch.h"
#include "Logger.h"
#include <vector>

void version() {
	#ifdef _DEBUG
		std::string configuration = "Debug";	
	#else
		std::string configuration = "Release";
	#endif

	SB_MESSAGE("Blocks (" << configuration << "): " << __DATE__ << ", " << __TIME__);
}

float getSeconds() {
	static sb::Stopwatch sw;
	return sw.getElapsedSeconds();
}

float getDeltaSeconds()
{
	static float lastElapsed = 0;
	float elapsed = getSeconds();
	float delta = elapsed - lastElapsed;
	lastElapsed = elapsed;
	return delta;
}

class Block : public sb::Drawable, public sb::Transformable {
	sb::Sprite _sprite;
	bool _dropping;

protected: 
	sb::Texture& getTexture() {
		static sb::Texture texture("Textures/Blocks.png");
		return texture;
	}

	void setTexture(char type) {
		if (type == 'i') 
			_sprite.setTexture(getTexture(), getTextureArea(2, 1));
		else if (type == 'j')
			_sprite.setTexture(getTexture(), getTextureArea(2, 2));
		else
			SB_ERROR("Tetromino type " << type << " not supported");
	}

	sb::IntRect getTextureArea(std::size_t row, std::size_t col) {
		static sb::Vector2i size(128, 128);
		return sb::IntRect(col * size.x, row * size.y, size.x, size.y);
	}

public:
	Block(char type) 
		: _dropping(true)
	{ 
		setTexture(tolower(type));
	}

	inline bool isDropping() const { return _dropping; }

	inline void freeze() { _dropping = false; }

	virtual void draw(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault()) {
		states.transform *= getTransform();
		target.draw(_sprite, states);
	}
};

class Board : public sb::Drawable, public sb::Transformable {
	struct Slot {
		Block* block = NULL;
		bool active = false;
	};

	std::size_t _numRows;
	std::size_t _numCols;
	std::vector<Slot> _slots;
	float _dropIntervalInSeconds;
	float _secondsSinceLastDrop;

protected:
	inline Slot& getSlot(std::size_t row, std::size_t col) { 
		return _slots[row * _numCols + col]; 
	}

	void spawnITetromino() {
		spawnBlock('i', _numRows - 1, _numCols / 2 - 2);
		spawnBlock('i', _numRows - 1, _numCols / 2 - 1);
		spawnBlock('i', _numRows - 1, _numCols / 2 + 0);
		spawnBlock('i', _numRows - 1, _numCols / 2 + 1);
	}
	
	void spawnJTetromino() {
		spawnBlock('j', _numRows - 1, _numCols / 2 - 2);
		spawnBlock('j', _numRows - 2, _numCols / 2 - 2);
		spawnBlock('j', _numRows - 2, _numCols / 2 - 1);
		spawnBlock('j', _numRows - 2, _numCols / 2 + 0);
	}

	void spawnMTetromino() {
		spawnBlock('i', _numRows - 1, _numCols / 2);
	}

	void spawnBlock(char type, std::size_t row, std::size_t col) {
		Slot& slot = getSlot(row, col);
		slot.block = new Block(type);
		initBlock(*slot.block, row, col);
		slot.active = true;
	}

	void initBlock(Block& block, std::size_t row, std::size_t col) {
		block.setScale(getSlotSize());
		setBlockPosition(block, row, col);
	}

	inline sb::Vector2f getSlotSize() {
		return sb::Vector2f(1.f/ _numCols, 1.f / _numRows);
	}

	void setBlockPosition(Block& block, std::size_t row, std::size_t col) {
		sb::Vector2f slotSize = getSlotSize();
		sb::Vector2f pos = sb::Vector2f((col + 0.5f) * slotSize.x - 0.5f, (row + 0.5f) * slotSize.y - 0.5f);
		block.setPosition(pos);
	}

	void dropBlocks(float ds) {
		_secondsSinceLastDrop += ds;
		while (_secondsSinceLastDrop > _dropIntervalInSeconds) {
			dropAllBlocks();
			_secondsSinceLastDrop -= _dropIntervalInSeconds;
		}
	}

	void dropAllBlocks() {
		for (std::size_t i = 0; i < _slots.size(); i++) {
			if (_slots[i].active && _slots[i].block->isDropping())
				dropBlock(i);
		}
	}

	void dropBlock(std::size_t index) {
		std::size_t row = index / _numCols;
		std::size_t col = index % _numCols;
		Block& block = *_slots[index].block;

		if (mustFreeze(row, col)) 
			block.freeze();
		else 
			moveBlock(row, col, row - 1, col);
	}

	bool mustFreeze(std::size_t row, std::size_t col) {
		return row == 0 || getSlot(row - 1, col).active;
	}

	void moveBlock(std::size_t sourceRow, std::size_t sourceCol, std::size_t destRow, std::size_t destCol) {
		Slot& source = getSlot(sourceRow, sourceCol);
		Slot& dest = getSlot(destRow, destCol);
		dest.block = source.block;
		setBlockPosition(*dest.block, destRow, destCol);
		dest.active = true;
		source.block = NULL;
		source.active = false;
	}

public:
	Board(std::size_t numRows, std::size_t numColumns) 
		: _numRows(numRows), _numCols(numColumns), _slots(numRows * numColumns), 
		_dropIntervalInSeconds(1), _secondsSinceLastDrop(0)
	{ }

	inline std::size_t getNumRows() const { return _numRows; }

	inline std::size_t getNumColumns() const { return _numCols; }

	void spawnTetromino(char type_) {
		char type = tolower(type_);

		if (type == 'i')
			spawnITetromino();
		else if (type == 'j')
			spawnJTetromino();
		else if (type == 'm')
			spawnMTetromino();
		else
			SB_ERROR("Tetromino type " << type << " not supported");
	}

	void fill(char type) {
		for (std::size_t i = 0; i < _numRows; i++) {
			for (std::size_t j = 0; j < _numCols; j++)
				spawnBlock(type, i, j);
		}
	}

	void update(float ds) {
		dropBlocks(ds);
	}

	virtual void draw(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault()) {
		states.transform *= getTransform();
		for (std::size_t i = 0; i < _slots.size(); i++) {
			if (_slots[i].active)
				target.draw(_slots[i].block, states);
		}
	}
};

void game(int desktopWindowWidth, float desktopAspect) {
	sb::DrawBatch batch(2048);
	sb::Window window(desktopWindowWidth, int(desktopWindowWidth * desktopAspect));
	Board board(15, 10);

	board.spawnTetromino('j');
	board.setScale(1, board.getNumRows() / float(board.getNumColumns()));

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		board.update(ds);

		window.clear(sb::Color(1, 1, 1, 1));
		batch.draw(board);
		window.draw(batch);
		window.display();
	}
}

int main() {
	version();
	
	//demo();

	game(400, 3 / 2.f);
	
	return 0;
}
