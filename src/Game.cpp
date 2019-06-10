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
	sb::DrawStates drawStates;

	BoardDrawStates(const sb::Vector2f& boardSize_, std::size_t numBoardRows_, std::size_t numBoardCols_, sb::DrawStates states_)
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

	inline void setRow(std::size_t row) { _row = row; }

	inline void setCol(std::size_t col) { _col = col; }

	inline sb::Vector2i getPosition() const { return sb::Vector2i(_row, _col); }

	void setPosition(sb::Vector2i position) { 
		_row = position.x;
		_col = position.y;
	}

	void translate(std::size_t numRows, std::size_t numCols) {
		_row += numRows;
		_col += numCols;
	}

	void drawOnBoard(sb::DrawTarget& target, BoardDrawStates boardStates) {
		sb::Vector2f cellSize(boardStates.boardSize.x / boardStates.numBoardCols, 
			boardStates.boardSize.y / boardStates.numBoardRows);
		sb::Vector2f halfBoardSize = 0.5f * boardStates.boardSize;
		sb::Vector2f offset(-halfBoardSize.x + cellSize.x / 2, -halfBoardSize.y + cellSize.y / 2);
		sb::Vector2f position(_col * cellSize.x + offset.x, _row * cellSize.y + offset.y);
		sb::Transform boardTransform(position, cellSize, 0);
		boardStates.drawStates.transform = boardStates.drawStates.transform * boardTransform;

		target.draw(_sprite, boardStates.drawStates);
	}
};

class Tetromino {
	std::vector<Block> _originalBlocks;
	std::vector<Block> _blocks;
	sb::Vector2i _position;
	std::size_t _numRotationSteps;
	char _type;

protected:
	void spawnBlocks() {
		_originalBlocks.clear();
		_blocks.clear();

		if (_type == 'i')
			spawnIBlocks();
		else if (_type == 'j')
			spawnJBlocks();
		else if (_type == 'm')
			spawnMBlocks();
		else 
			SB_ERROR("Invalid Tetromino type " << _type);

		_originalBlocks = _blocks;
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


	void setBlockTransform(std::size_t blockIndex, const sb::Vector2i& pos, std::size_t numRotationSteps_) {
		sb::Vector2i blockPosition = _originalBlocks[blockIndex].getPosition();
		rotatePosition(blockPosition, numRotationSteps_);
		blockPosition += pos;
		_blocks[blockIndex].setPosition(blockPosition);
	}

	void rotatePosition(sb::Vector2i& pos, std::size_t numRotationSteps_) {
		if (_numRotationSteps == 1)
			pos = sb::Vector2i(pos.y, -pos.x);
		else if (_numRotationSteps == 2)
			pos = sb::Vector2i(-pos.x, -pos.y);
		else if (_numRotationSteps == 3)
			pos = sb::Vector2i(-pos.y, pos.x);
	}

public:
	Tetromino(char type_ = 'i', std::size_t row = 0, std::size_t col = 0)
		: _position(row, col), _numRotationSteps(0), _type(tolower(type_))
	{
		spawnBlocks();
	}

	Tetromino(const Tetromino& other)
		: _position(other._position), _numRotationSteps(other._numRotationSteps), _type(other._type)
	{
		spawnBlocks();
	}

	Block& operator[](std::size_t index) { return _blocks[index]; }

	inline const std::vector<Block>& getBlocks() const { return _blocks; }

	inline std::vector<Block>& getBlocks() { return _blocks; }

	inline std::size_t getBlockCount() const { return _blocks.size(); }

	inline sb::Vector2i getPosition() const { return _position; }

	inline char getType() const { return _type; }

	inline std::size_t getNumRotationSteps() const { return _numRotationSteps; }

	void setPosition(sb::Vector2i position_) { 
		_position = position_;
		for (std::size_t i = 0; i < _blocks.size(); i++)
			setBlockTransform(i, _position, _numRotationSteps);
	}

	void rotateStep() { 
		_numRotationSteps = (_numRotationSteps + 1) % 4; 
		for (std::size_t i = 0; i < _blocks.size(); i++)
			setBlockTransform(i, _position, _numRotationSteps);
	}

	void translate(std::size_t numRows, std::size_t numCols) {
		setPosition(sb::Vector2i(_position.x + numRows, _position.y + numCols));
	}

	std::size_t computeHeight() {
		int max = _originalBlocks.empty() ? -1 : _originalBlocks[0].getRow();
		for (std::size_t i = 0; i < _originalBlocks.size(); i++) {
			if ((int)_originalBlocks[i].getRow() > max)
				max = _originalBlocks[i].getRow();
		}

		return max + 1;
	}

	void drawOnBoard(sb::DrawTarget& target, BoardDrawStates boardStates) {
		for (std::size_t i = 0; i < _blocks.size(); i++) {
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
			if (!isPositionAllowed(tetromino[i].getPosition()))
				return false;
		}

		return true;
	}

	inline bool isPositionAllowed(sb::Vector2i position) {
		return position.x >= 0 && position.x < (int)_numRows && 
			position.y >= 0 && position.y < (int)_numCols && isPositionEmpty(position);
	}

	bool isPositionEmpty(sb::Vector2i position) {
		for (std::size_t i = 0; i < _blocks.size(); i++) {
			if (_blocks[i].getRow() == position.x && _blocks[i].getCol() == position.y)
				return false;
		}

		return true;
	}

	bool canTetrominoRotate() {
		Tetromino rotatedTetromino(_tetromino);
		rotatedTetromino.rotateStep();
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
		for (std::size_t i = 0; i < tetrominoBlocks.size(); i++) 
			_blocks.push_back(tetrominoBlocks[i]);	
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
			_tetromino.rotateStep();
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

		if (_hasTetromino) {
			if (_tetromino.getNumRotationSteps() == 1)
				std::cout << "hit" << std::endl;
			_tetromino.drawOnBoard(target, boardStates);
		}
	}
};

struct Scene : public sb::Drawable {
	sb::DrawBatch batch = sb::DrawBatch(2048);
	Board board = Board(15, 10, 1.0f, { 'i' });
	
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
