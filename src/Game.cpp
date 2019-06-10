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

struct BoardDrawStates {
	sb::Vector2f boardSize;
	std::size_t numBoardRows;
	std::size_t numBoardCols;
	sb::Vector2i translation;
	sb::DrawStates drawStates;

	BoardDrawStates(const sb::Vector2f boardSize_, std::size_t numBoardRows_, std::size_t numBoardCols_, sb::DrawStates states_)
		: boardSize(boardSize_), numBoardRows(numBoardRows_), numBoardCols(numBoardCols_), drawStates(states_)
	{ }
};

class Block {
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

	void translate(std::size_t numRows, std::size_t numCols) {
		_row += numRows;
		_col += numCols;
	}

	void drawOnBoard(sb::DrawTarget& target, BoardDrawStates boardStates) {
		sb::Vector2i boardPosition(_row + boardStates.translation.x, _col + boardStates.translation.y);
		sb::Vector2f cellSize(boardStates.boardSize.x / boardStates.numBoardCols, 
			boardStates.boardSize.y / boardStates.numBoardRows);
		sb::Vector2f halfBoardSize = 0.5f * boardStates.boardSize;
		sb::Vector2f offset(-halfBoardSize.x + cellSize.x / 2, -halfBoardSize.y + cellSize.y / 2);
		sb::Vector2f position(boardPosition.y * cellSize.x + offset.x, boardPosition.x * cellSize.y + offset.y);
		sb::Transform boardTransform(position, cellSize, 0);
		boardStates.drawStates.transform = boardTransform;

		target.draw(_sprite, boardStates.drawStates);
	}
};

class Tetromino {
	std::vector<Block> _blocks;
	sb::Vector2i position;
	std::size_t numRotationSteps;
	char type;

protected:
	void rotateGridPoint(sb::Vector2i& gridPoint, std::size_t numRotationSteps_) {
		if (numRotationSteps == 1)
			gridPoint = sb::Vector2i(gridPoint.y, -gridPoint.x);
		else if (numRotationSteps == 2)
			gridPoint = sb::Vector2i(-gridPoint.x, -gridPoint.y);
		else if (numRotationSteps == 3)
			gridPoint = sb::Vector2i(-gridPoint.y, gridPoint.x);
	}

	void spawnBlocks() {
		_blocks.clear();

		if (type == 'i')
			spawnIBlocks();
		else if (type == 'j')
			spawnJBlocks();
		else if (type == 'm')
			spawnMBlocks();
		else 
			SB_ERROR("Invalid Tetromino type " << type);
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

public:
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

	inline std::vector<Block>& getBlocks() { return _blocks; }

	inline std::size_t getBlockCount() const { return _blocks.size(); }

	inline sb::Vector2i getPosition() const { return position; }

	inline char getType() const { return type; }

	sb::Vector2i getBlockPosition(std::size_t index) {
		sb::Vector2i point(_blocks[index].getRow(), _blocks[index].getCol());
		rotateGridPoint(point, numRotationSteps);
		point += position;
		return point;
	}

	inline void setPosition(sb::Vector2i position_) { position = position_; }

	inline void rotate90Degrees() { numRotationSteps = (numRotationSteps + 1) % 4; }

	void translate(std::size_t numRows, std::size_t numCols) {
		position += sb::Vector2i(numRows, numCols);
	}

	std::size_t computeHeight() {
		int max = _blocks.empty() ? -1 : _blocks[0].getRow();
		for (std::size_t i = 0; i < _blocks.size(); i++) {
			if ((int)_blocks[i].getRow() > max)
				max = _blocks[i].getRow();
		}

		return max + 1;
	}

	void drawOnBoard(sb::DrawTarget& target, BoardDrawStates boardStates) {
		for (std::size_t i = 0; i < _blocks.size(); i++) {
			boardStates.translation = position;
			_blocks[i].drawOnBoard(target, boardStates);
		}
	}
};

class Board : public sb::Drawable, public sb::Transformable {
	std::size_t _numRows;
	std::size_t _numCols;
	std::vector<Block> _blocks;
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
		newTetromino.setPosition(sb::Vector2i(_numRows - newTetromino.computeHeight(), _numCols / 2));
		
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
		return row >= 0 && row < (int)_numRows && col >= 0 && col < (int)_numCols && isPositionEmpty(row, col);
	}

	bool isPositionEmpty(std::size_t row, std::size_t col) {
		for (std::size_t i = 0; i < _blocks.size(); i++) {
			if (_blocks[i].getRow() == row && _blocks[i].getCol() == col)
				return false;
		}

		return true;
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
		std::cout << _tetromino.getPosition().x << " " << _tetromino.getPosition().y << std::endl;

		if (canTetrominoDrop())
			_tetromino.translate(-1, 0);
		else {
			std::cout << "freeze" << std::endl;
			freezeTetromino();
		}

	}

	bool canTetrominoDrop() {
		Tetromino droppedTetromino(_tetromino);
		droppedTetromino.translate(-1, 0);
		return isTetrominoAllowed(droppedTetromino);
	}

	void freezeTetromino() {
		_hasTetromino = false;
		const sb::Vector2i tetrominoPosition = _tetromino.getPosition();
		const std::vector<Block>& tetrominoBlocks = _tetromino.getBlocks();
		for (std::size_t i = 0; i < tetrominoBlocks.size(); i++) {
			_blocks.push_back(tetrominoBlocks[i]);
			_blocks[_blocks.size() - 1].translate(tetrominoPosition.x, tetrominoPosition.y);
		}
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
			for (std::size_t j = 0; j < _numCols; j++)
				_blocks.push_back(Block(type, i, j));
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
		BoardDrawStates boardStates(getScale(), _numRows, _numCols, states);

		for (std::size_t i = 0; i < _blocks.size(); i++)
			_blocks[i].drawOnBoard(target, boardStates);

		if (_hasTetromino)
			_tetromino.drawOnBoard(target, boardStates);
	}
};

struct Scene : public sb::Drawable {
	sb::DrawBatch batch = sb::DrawBatch(2048);
	Board board = Board(15, 10, 0.1f, { 'm' });

	Scene() { 
	}

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
