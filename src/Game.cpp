#include "Demo.h"
#include "Window.h"
#include "DrawBatch.h"
#include "Input.h"
#include "Sprite.h"
#include "Stopwatch.h"
#include "Logger.h"
#include <vector>
#include <algorithm>

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
			SB_ERROR("Invalid Tetromino type " << type);
	}

	sb::IntRect getTextureArea(std::size_t row, std::size_t col) {
		static sb::Vector2i size(128, 128);
		return sb::IntRect(col * size.x, row * size.y, size.x, size.y);
	}

public:
	Block(char type)
	{ 
		setTexture(tolower(type));
	}

	virtual void draw(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault()) {
		states.transform *= getTransform();
		target.draw(_sprite, states);
	}
};

class Board : public sb::Drawable, public sb::Transformable {
	struct Item {
		Block block;
		std::size_t row, col;
		Item(char type, std::size_t row_, std::size_t col_)
			: block(type), row(row_), col(col_) 
		{ }
	};

	struct Tetromino {
		std::vector<Item> items;
		char type;
	};

	std::size_t _numRows;
	std::size_t _numCols;
	std::vector<Item> _items;
	bool _hasDroppingTetromino;
	Tetromino _droppingTetromino;
	float _stepIntervalInSeconds;
	float _secondsSinceLastStep;

protected:
	void spawnITetromino() {
		spawnBlock('i', _numRows - 1, _numCols / 2 - 1);
		spawnBlock('i', _numRows - 1, _numCols / 2 - 2);
		spawnBlock('i', _numRows - 1, _numCols / 2 + 0);
		spawnBlock('i', _numRows - 1, _numCols / 2 + 1);
	}
	
	void spawnJTetromino() {
		spawnBlock('j', _numRows - 2, _numCols / 2 - 2);
		spawnBlock('j', _numRows - 1, _numCols / 2 - 2);
		spawnBlock('j', _numRows - 2, _numCols / 2 - 1);
		spawnBlock('j', _numRows - 2, _numCols / 2 + 0);
	}

	void spawnMTetromino() {
		spawnBlock('i', _numRows - 1, _numCols / 2);
	}

	void spawnBlock(char type, std::size_t row, std::size_t col) {
		Item item(type, row, col);
		initBlock(item.block, item.row, item.col);
		_droppingTetromino.items.push_back(item);
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

	bool canTetrominoRotate() {
		Item& pivotItem = _droppingTetromino.items[0];
		for (std::size_t i = 0; i < _droppingTetromino.items.size(); i++) {
			if (!canItemRotate(_droppingTetromino.items[i], pivotItem.row, pivotItem.col))
				return false;
		}

		return true;
	}

	bool canItemRotate(Item& item, std::size_t pivotRow, std::size_t pivotCol) {
		sb::Vector2i rotatedPos = rotated(item.row, item.col, pivotRow, pivotCol);
		return isPositionAllowed(rotatedPos.x, rotatedPos.y);
	}

	sb::Vector2i rotated(std::size_t row, std::size_t col, std::size_t pivotRow, std::size_t pivotCol) {
		sb::Vector2i relative(row - pivotRow, col - pivotCol);
		sb::Vector2i rotated(-relative.y, relative.x);
		return sb::Vector2i(rotated.x + pivotRow, rotated.y + pivotCol);
	}

	void rotateDroppingTetromino() {
		for (std::size_t i = 0; i < _droppingTetromino.items.size(); i++) 
			rotateItem(_droppingTetromino.items[i]);
	}

	void rotateItem(Item& item) {
		Item& pivotItem = _droppingTetromino.items[0];
		sb::Vector2i rotatedPos = rotated(item.row, item.col, pivotItem.row, pivotItem.col);
		setItemPosition(item, rotatedPos.x, rotatedPos.y);
	}

	void step(float ds) {
		if (!_hasDroppingTetromino)
			spawn();
		else
			dropTetromino();
	}

	void spawn() {
		static const std::vector<char> types = { 'i', 'j' };
		spawnTetromino(types[rand() % types.size()]);
	}

	void dropTetromino() {
		if (canTetrominoDrop())
			dropTetrominoItems();
		else
			freezeDroppingTetromino();
	}

	bool canTetrominoDrop() {
		for (std::size_t i = 0; i < _droppingTetromino.items.size(); i++) {
			Item& item = _droppingTetromino.items[i];
			if (!isPositionAllowed(item.row, item.col))
				return false;
		}

		return true;
	}

	inline bool isPositionAllowed(int row, int col) {
		return row > 0 && row < (int)_numRows && col > 0 && col < (int)_numCols && isPositionEmpty(row - 1, col);
	}

	bool isPositionEmpty(std::size_t row, std::size_t col) {
		for (std::size_t i = 0; i < _items.size(); i++) {
			if (_items[i].row == row && _items[i].col == col)
				return false;
		}

		return true;
	}

	void dropTetrominoItems() {
		for (std::size_t i = 0; i < _droppingTetromino.items.size(); i++) {
			Item& item = _droppingTetromino.items[i];
			setItemPosition(item, --item.row, item.col);
		}
	}

	void setItemPosition(Item& item, std::size_t row, std::size_t col) {
		item.row = row;
		item.col = col;
		setBlockPosition(item.block, item.row, item.col);
	}

	void freezeDroppingTetromino() {
		_hasDroppingTetromino = false;
		_items.insert(_items.end(), _droppingTetromino.items.begin(), _droppingTetromino.items.end());
	}

public:
	Board(std::size_t numRows, std::size_t numColumns) 
		: _numRows(numRows), _numCols(numColumns), _hasDroppingTetromino(false),
		_stepIntervalInSeconds(1), _secondsSinceLastStep(0)
	{
		setScale(1, (float)_numRows / (float)_numCols);
		spawn();
	}

	inline std::size_t getNumRows() const { return _numRows; }

	inline std::size_t getNumColumns() const { return _numCols; }

	void spawnTetromino(char type_) {
		char type = tolower(type_);
		_hasDroppingTetromino = true;
		_droppingTetromino.items.clear();
		_droppingTetromino.type = type;

		if (type == 'i')
			spawnITetromino();
		else if (type == 'j')
			spawnJTetromino();
		else if (type == 'm')
			spawnMTetromino();
		else
			SB_ERROR("Invalid Tetromino type " << type);
	}

	void fill(char type) {
		for (std::size_t i = 0; i < _numRows; i++) {
			for (std::size_t j = 0; j < _numCols; j++)
				spawnBlock(type, i, j);
		}
	}

	void rotateTetromino() {
		if (canTetrominoRotate())
			rotateDroppingTetromino();
	}

	void update(float ds) {
		_secondsSinceLastStep += ds;
		while (_secondsSinceLastStep > _stepIntervalInSeconds) {
			step(ds);
			_secondsSinceLastStep -= _stepIntervalInSeconds;
		}
	}

	virtual void draw(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault()) {
		states.transform *= getTransform();
		for (std::size_t i = 0; i < _items.size(); i++) 
			target.draw(_items[i].block, states);
		for (std::size_t i = 0; i < _droppingTetromino.items.size(); i++)
			target.draw(_droppingTetromino.items[i].block, states);
		
	}
};

struct Scene : public sb::Drawable {
	sb::DrawBatch batch = sb::DrawBatch(2048);
	Board board = Board(15, 10);

	void input() {
		if (sb::Input::isKeyGoingDown(sb::KeyCode::r))
			board.rotateTetromino();
	}

	void update() {
		float ds = getDeltaSeconds();
		input();
		board.update(ds);
	}

	virtual void draw(sb::DrawTarget& target, sb::DrawStates drawStates = sb::DrawStates::getDefault()) {
		batch.draw(board);
		target.draw(batch);
	}
};

void game() {
	sb::Window window(400, int(1.5f * 400));
	Scene scene;

	while (window.isOpen()) {
		sb::Input::update();
		window.update();
		scene.update();

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(scene);
		window.display();
	}
}
