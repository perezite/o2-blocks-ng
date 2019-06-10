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

class Block : public sb::Drawable {
	sb::Sprite _sprite;
	char _type;
	std::size_t _row;
	std::size_t _col;

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
	Block(char type, std::size_t row, std::size_t col)
		: _type(type), _row(row), _col(col)
	{ 
		setTexture(tolower(type));
	}

	inline std::size_t getRow() const { return _row; }

	inline std::size_t getCol() const { return _col; }

	virtual void draw(sb::DrawTarget& target, sb::DrawStates drawStates = sb::DrawStates::getDefault()) {
		target.draw(_sprite);
	}
};

struct BoardItem {
	Block block;
	std::size_t row, col;
	BoardItem(char type, std::size_t row_, std::size_t col_)
		: block(type, row, col), row(row_), col(col_) 
	{ }
};

struct Tetromino {
	std::vector<Block> _blocks;
	sb::Vector2i position;
	std::size_t numRotationSteps;
	char type;

	Tetromino(char type_ = 'i', std::size_t row = 0, std::size_t col = 0)
		: position(row, col), numRotationSteps(0), type(tolower(type_))
	{
		spawnBlocks();
	}

	Tetromino(const Tetromino& other)
		: position(other.position), numRotationSteps(other.numRotationSteps), type(other.type)
	{
		spawnBlocks();
	}

	inline const std::vector<Block>& getBlocks() const { return _blocks; }

	inline void setPosition(sb::Vector2i position_) { position = position_; }

	inline std::size_t getBlockCount() const { return _blocks.size(); }

	inline char getType() const { return type; }

	inline sb::Vector2i getPosition() const { return position; }

	inline void rotate90Degrees() { numRotationSteps = (numRotationSteps + 1) % 4; }

	sb::Vector2i getBlockPosition(std::size_t index) {
		sb::Vector2i point(_blocks[index].getRow(), _blocks[index].getCol());
		rotate(point, numRotationSteps);
		point += position;
		return point;
	}

	std::size_t computeHeight() {
		int max = _blocks.empty() ? -1 : _blocks[0].getRow();
		for (std::size_t i = 0; i < _blocks.size(); i++) {
			if ((int)_blocks[i].getRow() > max)
				max = _blocks[i].getRow();
		}

		return max + 1;
	}

	void rotate(sb::Vector2i& point, std::size_t numRotationSteps_) {
		if (numRotationSteps == 1)
			point = sb::Vector2i(point.y, -point.x);
		else if (numRotationSteps == 2)
			point = sb::Vector2i(-point.x, -point.y);
		else if (numRotationSteps == 3)
			point = sb::Vector2i(-point.y, point.x);
	}

	void translate(std::size_t numRows, std::size_t numCols) {
		position += sb::Vector2i(numRows, numCols);
	}

	void spawnBlocks() {
		_blocks.clear();

		if (type == 'i')
			spawnIBlocks();
		else if (type == 'j')
			spawnJBlocks();
		else if (type == 'm')
			spawnMBlocks();
	}

	void spawnIBlocks() {
		_blocks.push_back(Block('i', 0, +0));
		_blocks.push_back(Block('i', 0, -1));
		_blocks.push_back(Block('i', 0, +1));
		_blocks.push_back(Block('i', 0, +2));
	}

	void spawnJBlocks() {
		_blocks.push_back(Block('j', 0, +0));
		_blocks.push_back(Block('j', 1, +0));
		_blocks.push_back(Block('j', 0, +1));
		_blocks.push_back(Block('j', 0, +2));
	}

	void spawnMBlocks() {
		_blocks.push_back(Block('m', 0, 0));
	}
};

class Board : public sb::Drawable, public sb::Transformable {
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
	void spawnRandomTetromino() {
		spawnTheTetromino(_types[rand() % _types.size()]);
	}

	void spawnTheTetromino(char type) {
		_hasTetromino = true;
		Tetromino newTetromino(type);
		newTetromino.setPosition(sb::Vector2i(_numRows - 1 - newTetromino.computeHeight(), _numCols / 2));
		
		if (isTetrominoAllowed(newTetromino))
			_tetromino = newTetromino;	
		else 
			_isAlive = false;
	}

	bool isTetrominoAllowed(Tetromino& tetromino) {
		for (std::size_t i = 0; i < tetromino.getBlockCount(); i++) {
			if (!isPositionAllowed(tetromino.getBlockPosition(i)))
				return false;
		}

		return true;
	}

	inline bool isPositionAllowed(sb::Vector2i position) {
		return isPositionAllowed(position.x, position.y);
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

	inline sb::Vector2f getSlotSize() {
		return sb::Vector2f(1.f/ _numCols, 1.f / _numRows);
	}

	bool canTetrominoRotate() {
		Tetromino rotatedTetromino(_tetromino);
		rotatedTetromino.rotate90Degrees();
		return isTetrominoAllowed(rotatedTetromino);
	}

	void step(float ds) {
		if (!_hasTetromino)
			spawnRandomTetromino();
		else
			dropTetromino();
	}

	void dropTetromino() {
		if (canTetrominoDrop())
			_tetromino.translate(-1, 0);
		else
			freezeTetromino();
	}

	bool canTetrominoDrop() {
		Tetromino droppedTetromino(_tetromino);
		droppedTetromino.translate(-1, 0);
		return isTetrominoAllowed(droppedTetromino);
	}

	void freezeTetromino() {
		_hasTetromino = false;
		const std::vector<Block>& tetrominoItems = _tetromino.getBlocks();
		// _items.insert(_items.end(), tetrominoItems.begin(), tetrominoItems.end());
	}

public:
	Board(std::size_t numRows, std::size_t numColumns, float stepIntervalInSeconds = 1.f, std::vector<char> types = { 'i', 'j' })
		: _numRows(numRows), _numCols(numColumns), _hasTetromino(false),
		_stepIntervalInSeconds(stepIntervalInSeconds), _secondsSinceLastStep(0), _types(types)
	{
		setScale(1, (float)_numRows / (float)_numCols);
		spawnRandomTetromino();
	}

	inline std::size_t getNumRows() const { return _numRows; }

	inline std::size_t getNumColumns() const { return _numCols; }

	inline bool isAlive() const { return _isAlive; }

	void spawnTetromino(char type_) {
		char type = tolower(type_);
		spawnTheTetromino(type);
	}

	void rotateTetromino() {
		if (canTetrominoRotate())
			_tetromino.rotate90Degrees();
	}

	void fill(char type) {
		for (std::size_t i = 0; i < _numRows; i++) {
			/*for (std::size_t j = 0; j < _numCols; j++)
				spawnBlock(type, i, j);*/
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
		for (std::size_t i = 0; i < _tetromino._blocks.size(); i++)
			target.draw(_tetromino._blocks[i], states);
		
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
