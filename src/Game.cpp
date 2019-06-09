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

	void step(float ds) {
		if (!_hasDroppingTetromino)
			spawn();
		else
			dropTetromino();
	}

	void spawn() {
		static const std::vector<char> types = { 'i', 'j'};
		spawnTetromino(types[rand() % types.size()]);
	}

	void dropTetromino() {
		if (canTetrominoDrop())
			dropTetrominoItems();
		else
			freezeDroppingTetromino();
	}

	bool canTetrominoDrop() {
		for (std::size_t i = 0; i < _droppingTetromino.items.size(); i++)
			if (!canItemDrop(_droppingTetromino.items[i]))
				return false;

		return true;
	}

	inline bool canItemDrop(Item& item) {
		return item.row > 0 && isPositionEmpty(item.row - 1, item.col);
	}

	bool isPositionEmpty(std::size_t row, std::size_t col) {
		for (std::size_t i = 0; i < _items.size(); i++) {
			if (_items[i].row == row && _items[i].col == col)
				return false;
		}

		return true;
	}

	void dropTetrominoItems() {
		for (std::size_t i = 0; i < _droppingTetromino.items.size(); i++)
			dropItem(_droppingTetromino.items[i]);
	}

	void dropItem(Item& item) {
		item.row--;
		setBlockPosition(item.block, item.row, item.col);
	}

	void freezeDroppingTetromino() {
		_hasDroppingTetromino = false;
		_items.insert(_items.end(), _droppingTetromino.items.begin(), _droppingTetromino.items.end());
	}

public:
	Board(std::size_t numRows, std::size_t numColumns) 
		: _numRows(numRows), _numCols(numColumns), _hasDroppingTetromino(false),
		_stepIntervalInSeconds(0.1f), _secondsSinceLastStep(0)
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

void game() {
	sb::DrawBatch batch(2048);
	sb::Window window(400, int(1.5f * 400));
	Board board(15, 10);

	board.setScale(1, (float)board.getNumRows() / (float)board.getNumColumns());

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
