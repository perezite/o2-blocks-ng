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
		else if (type == 'm')
			_sprite.setTexture(getTexture(), getTextureArea(2, 1));
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

struct BoardItem {
	Block block;
	std::size_t row, col;
	BoardItem(char type, std::size_t row_, std::size_t col_)
		: block(type), row(row_), col(col_) 
	{ }
};

struct Tetromino {
	std::vector<BoardItem> items;
	char type;
};

class Board : public sb::Drawable, public sb::Transformable {
private:
	std::size_t _numRows;
	std::size_t _numCols;
	std::vector<BoardItem> _items;
	bool _hasTetromino;
	Tetromino _tetromino;
	float _stepIntervalInSeconds;
	float _secondsSinceLastStep;
	bool _isAlive;
	std::vector<char> _types;

protected:
	void spawnTetrominoBlocks(char type) {
		std::vector<sb::Vector2i> blockPositions;
		if (type == 'i')
			blockPositions = getITetrominoBlockPositions();
		else if (type == 'j')
			blockPositions = getJTetrominoBlockPositions();
		else if (type == 'm')
			blockPositions = getMTetrominoBlockPositions();
		else
			SB_ERROR("Invalid Tetromino type " << type);

		spawnTetrominoBlocks(type, blockPositions);
	}

	std::vector<sb::Vector2i> getITetrominoBlockPositions() {
		return std::vector<sb::Vector2i> {
			sb::Vector2i(_numRows - 1, _numCols / 2 - 1),
			sb::Vector2i(_numRows - 1, _numCols / 2 - 2),
			sb::Vector2i(_numRows - 1, _numCols / 2 + 0),
			sb::Vector2i(_numRows - 1, _numCols / 2 + 1) 
		};
	}
	
	std::vector<sb::Vector2i> getJTetrominoBlockPositions() {
		return std::vector<sb::Vector2i> {
			sb::Vector2i(_numRows - 2, _numCols / 2 - 2),
			sb::Vector2i(_numRows - 1, _numCols / 2 - 2),
			sb::Vector2i(_numRows - 2, _numCols / 2 - 1),
			sb::Vector2i(_numRows - 2, _numCols / 2 + 0) 
		};
	}

	std::vector<sb::Vector2i> getMTetrominoBlockPositions() {
		return std::vector<sb::Vector2i> {
			sb::Vector2i(_numRows - 1, _numCols / 2)
		};
	}

	void spawnTetrominoBlocks(char type, std::vector<sb::Vector2i> positions) {
		if (!canSpawnBlocks(positions)) {
			_isAlive = false;
			return;
		}

		for (std::size_t i = 0; i < positions.size(); i++) {
			spawnBlock(type, positions[i].x, positions[i].y);
		}
	}

	bool canSpawnBlocks(std::vector<sb::Vector2i> positions) {
		for (std::size_t i = 0; i < positions.size(); i++)
			if (!isPositionAllowed(positions[i].x, positions[i].y))
				return false;

		return true;
	}

	inline bool isPositionAllowed(int row, int col) {
		return row > 0 && row < (int)_numRows && col > 0 && col < (int)_numCols && isPositionEmpty(row - 1, col);
	}

	void spawnBlock(char type, std::size_t row, std::size_t col) {
		BoardItem item(type, row, col);
		initBlock(item.block, item.row, item.col);
		_tetromino.items.push_back(item);
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
		BoardItem& pivotItem = _tetromino.items[0];
		for (std::size_t i = 0; i < _tetromino.items.size(); i++) {
			if (!canItemRotate(_tetromino.items[i], pivotItem.row, pivotItem.col))
				return false;
		}

		return true;
	}

	bool canItemRotate(BoardItem& item, std::size_t pivotRow, std::size_t pivotCol) {
		sb::Vector2i rotatedPos = rotated(item.row, item.col, pivotRow, pivotCol);
		return isPositionAllowed(rotatedPos.x, rotatedPos.y);
	}

	sb::Vector2i rotated(std::size_t row, std::size_t col, std::size_t pivotRow, std::size_t pivotCol) {
		sb::Vector2i relative(row - pivotRow, col - pivotCol);
		sb::Vector2i rotated(-relative.y, relative.x);
		return sb::Vector2i(rotated.x + pivotRow, rotated.y + pivotCol);
	}

	void rotateTheTetromino() {
		for (std::size_t i = 0; i < _tetromino.items.size(); i++) 
			rotateItem(_tetromino.items[i]);
	}

	void rotateItem(BoardItem& item) {
		BoardItem& pivotItem = _tetromino.items[0];
		sb::Vector2i rotatedPos = rotated(item.row, item.col, pivotItem.row, pivotItem.col);
		setItemPosition(item, rotatedPos.x, rotatedPos.y);
	}

	void step(float ds) {
		if (!_hasTetromino)
			spawn();
		else
			dropTetromino();
	}

	void spawn() {
		spawnTetromino(_types[rand() % _types.size()]);
	}

	void dropTetromino() {
		if (canTetrominoDrop())
			dropTetrominoItems();
		else
			freezeTetromino();
	}

	bool canTetrominoDrop() {
		for (std::size_t i = 0; i < _tetromino.items.size(); i++) {
			BoardItem& item = _tetromino.items[i];
			if (!isPositionAllowed(item.row, item.col))
				return false;
		}

		return true;
	}

	bool isPositionEmpty(std::size_t row, std::size_t col) {
		for (std::size_t i = 0; i < _items.size(); i++) {
			if (_items[i].row == row && _items[i].col == col)
				return false;
		}

		return true;
	}

	void dropTetrominoItems() {
		for (std::size_t i = 0; i < _tetromino.items.size(); i++) {
			BoardItem& item = _tetromino.items[i];
			setItemPosition(item, --item.row, item.col);
		}
	}

	void setItemPosition(BoardItem& item, std::size_t row, std::size_t col) {
		item.row = row;
		item.col = col;
		setBlockPosition(item.block, item.row, item.col);
	}

	void freezeTetromino() {
		_hasTetromino = false;
		_items.insert(_items.end(), _tetromino.items.begin(), _tetromino.items.end());
	}

public:
	Board(std::size_t numRows, std::size_t numColumns, float stepIntervalInSeconds = 1.f, std::vector<char> types = { 'i', 'j' })
		: _numRows(numRows), _numCols(numColumns), _hasTetromino(false),
		_stepIntervalInSeconds(stepIntervalInSeconds), _secondsSinceLastStep(0), _types(types)
	{
		setScale(1, (float)_numRows / (float)_numCols);
		spawn();
	}

	inline std::size_t getNumRows() const { return _numRows; }

	inline std::size_t getNumColumns() const { return _numCols; }

	inline bool isAlive() const { return _isAlive; }

	void spawnTetromino(char type_) {
		char type = tolower(type_);
		_hasTetromino = true;
		_tetromino.items.clear();
		_tetromino.type = type; 
		spawnTetrominoBlocks(type);
	}

	void rotateTetromino() {
		if (canTetrominoRotate())
			rotateTheTetromino();
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
		for (std::size_t i = 0; i < _tetromino.items.size(); i++)
			target.draw(_tetromino.items[i].block, states);
		
	}
};

struct Scene : public sb::Drawable {
	sb::DrawBatch batch = sb::DrawBatch(2048);
	Board board = Board(2, 2, 1, { 'm' });

	void input() {
		if (sb::Input::isKeyGoingDown(sb::KeyCode::r))
			board.rotateTetromino();
	}

	void update() {
		float ds = getDeltaSeconds();
		if (board.isAlive()) {
			input();
			board.update(ds);
		} 
		else
			SB_MESSAGE("You DIED!!");
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
