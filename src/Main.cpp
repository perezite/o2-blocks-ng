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

class Stone : public sb::Drawable, public sb::Transformable {
	sb::Sprite _sprite;
	bool _dropping;

protected: 
	sb::Texture& getTexture() {
		static sb::Texture texture("Textures/Stones.png");
		return texture;
	}

	void setTexture(char letter) {
		if (letter == 'i') 
			_sprite.setTexture(getTexture(), getTextureArea(2, 1));
		else if (letter == 'j')
			_sprite.setTexture(getTexture(), getTextureArea(2, 2));
		else
			SB_ERROR("Tetromino letter " << letter << " not supported");
	}

	sb::IntRect getTextureArea(std::size_t row, std::size_t col) {
		static sb::Vector2i size(128, 128);
		return sb::IntRect(col * size.x, row * size.y, size.x, size.y);
	}

public:
	Stone(char letter) 
		: _dropping(true)
	{ 
		setTexture(tolower(letter));
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
		Stone* stone = NULL;
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
		spawnStone('i', _numRows - 1, _numCols / 2 - 2);
		spawnStone('i', _numRows - 1, _numCols / 2 - 1);
		spawnStone('i', _numRows - 1, _numCols / 2 + 0);
		spawnStone('i', _numRows - 1, _numCols / 2 + 1);
	}
	
	void spawnJTetromino() {
		spawnStone('j', _numRows - 1, _numCols / 2 - 2);
		spawnStone('j', _numRows - 2, _numCols / 2 - 2);
		spawnStone('j', _numRows - 2, _numCols / 2 - 1);
		spawnStone('j', _numRows - 2, _numCols / 2 + 0);
	}

	void spawnMTetromino() {
		spawnStone('i', _numRows - 1, _numCols / 2);
	}

	void spawnStone(char letter, std::size_t row, std::size_t col) {
		Slot& slot = getSlot(row, col);
		slot.stone = new Stone(letter);
		initStone(*slot.stone, row, col);
		slot.active = true;
	}

	void initStone(Stone& stone, std::size_t row, std::size_t col) {
		stone.setScale(getSlotSize());
		setStonePosition(stone, row, col);
	}

	inline sb::Vector2f getSlotSize() {
		return sb::Vector2f(1.f/ _numCols, 1.f / _numRows);
	}

	void setStonePosition(Stone& stone, std::size_t row, std::size_t col) {
		sb::Vector2f slotSize = getSlotSize();
		sb::Vector2f pos = sb::Vector2f((col + 0.5f) * slotSize.x - 0.5f, (row + 0.5f) * slotSize.y - 0.5f);
		stone.setPosition(pos);
	}

	void dropStones(float ds) {
		_secondsSinceLastDrop += ds;
		while (_secondsSinceLastDrop > _dropIntervalInSeconds) {
			dropAllStones();
			_secondsSinceLastDrop -= _dropIntervalInSeconds;
		}
	}

	void dropAllStones() {
		for (std::size_t i = 0; i < _slots.size(); i++) {
			if (_slots[i].active && _slots[i].stone->isDropping())
				dropStone(i);
		}
	}

	void dropStone(std::size_t index) {
		std::size_t row = index / _numCols;
		std::size_t col = index % _numCols;
		Stone& stone = *_slots[index].stone;

		if (mustFreeze(row, col)) 
			stone.freeze();
		else 
			moveStone(row, col, row - 1, col);
	}

	bool mustFreeze(std::size_t row, std::size_t col) {
		return row == 0 || getSlot(row - 1, col).active;
	}

	void moveStone(std::size_t sourceRow, std::size_t sourceCol, std::size_t destRow, std::size_t destCol) {
		Slot& source = getSlot(sourceRow, sourceCol);
		Slot& dest = getSlot(destRow, destCol);
		dest.stone = source.stone;
		setStonePosition(*dest.stone, destRow, destCol);
		dest.active = true;
		source.stone = NULL;
		source.active = false;
	}

public:
	Board(std::size_t numRows, std::size_t numColumns) 
		: _numRows(numRows), _numCols(numColumns), _slots(numRows * numColumns), 
		_dropIntervalInSeconds(1), _secondsSinceLastDrop(0)
	{ }

	inline std::size_t getNumRows() const { return _numRows; }

	inline std::size_t getNumColumns() const { return _numCols; }

	void spawnTetromino(char letter_) {
		char letter = tolower(letter_);

		if (letter == 'i')
			spawnITetromino();
		else if (letter == 'j')
			spawnJTetromino();
		else if (letter == 'm')
			spawnMTetromino();
		else
			SB_ERROR("Tetromino letter " << letter << " not supported");
	}

	void fill(char letter) {
		for (std::size_t i = 0; i < _numRows; i++) {
			for (std::size_t j = 0; j < _numCols; j++)
				spawnStone(letter, i, j);
		}
	}

	void update(float ds) {
		dropStones(ds);
	}

	virtual void draw(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault()) {
		states.transform *= getTransform();
		for (std::size_t i = 0; i < _slots.size(); i++) {
			if (_slots[i].active)
				target.draw(_slots[i].stone, states);
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
