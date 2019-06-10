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
	sb::Vector2f boardScale;
	sb::Vector2i boardSize;
	sb::DrawStates drawStates;

	BoardDrawStates(const sb::Vector2f& boardScale_, const sb::Vector2i& boardSize_, sb::DrawStates states_)
		: boardScale(boardScale_), boardSize(boardSize_), drawStates(states_)
	{ }
};

class Block {
	sb::Sprite _sprite;
	char _type;
	sb::Vector2i _position;

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
	Block(char type, std::size_t col, std::size_t row)
		: _type(type), _position(col, row)
	{
		setTexture(tolower(type));
	}

	inline sb::Vector2i getPosition() const { return _position; }

	inline void setPosition(sb::Vector2i position) { _position = position; }

	void drawOnBoard(sb::DrawTarget& target, BoardDrawStates boardStates) {
		sb::Vector2f cellSize(boardStates.boardScale.x / boardStates.boardSize.x, 
			boardStates.boardScale.y / boardStates.boardSize.y);
		sb::Vector2f halfBoardSize = 0.5f * boardStates.boardScale;
		sb::Vector2f offset(-halfBoardSize.x + cellSize.x / 2, -halfBoardSize.y + cellSize.y / 2);
		sb::Vector2f position(_position.x * cellSize.x + offset.x, _position.y * cellSize.y + offset.y);
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

	void translate(const sb::Vector2i& translation) {
		setPosition(sb::Vector2i(_position.x + translation.x, _position.y + translation.y));
	}

	std::size_t computeHeight() {
		int max = _originalBlocks.empty() ? -1 : _originalBlocks[0].getPosition().y;
		for (std::size_t i = 0; i < _originalBlocks.size(); i++) {
			if ((int)_originalBlocks[i].getPosition().y > max)
				max = _originalBlocks[i].getPosition().y;
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
	sb::Vector2i _gridSize;
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
		newTetromino.setPosition(sb::Vector2i(_gridSize.x / 2, _gridSize.y - newTetromino.computeHeight()));
		
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
		return position.x >= 0 && position.x < _gridSize.x && 
			position.y >= 0 && position.y < _gridSize.y && isPositionEmpty(position);
	}

	bool isPositionEmpty(sb::Vector2i position) {
		for (std::size_t i = 0; i < _blocks.size(); i++) {
			if (_blocks[i].getPosition().x == position.x && _blocks[i].getPosition().y == position.y)
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
		if (canTetrominoDrop())
			_tetromino.translate(sb::Vector2i(0, -1));
		else 
			freezeTetromino();
	}

	bool canTetrominoDrop() {
		Tetromino droppedTetromino(_tetromino);
		droppedTetromino.translate(sb::Vector2i(0, -1));
		return isTetrominoAllowed(droppedTetromino);
	}

	void freezeTetromino() {
		_hasTetromino = false;
		const sb::Vector2i tetrominoPosition = _tetromino.getPosition();
		for (std::size_t i = 0; i < _tetromino.getBlockCount(); i++) 
			_blocks.push_back(_tetromino[i]);	
	}

public:
	Board(std::size_t numCols, std::size_t numRows, float stepIntervalInSeconds = 1.f, std::vector<char> types = { 'i', 'j' })
		: _gridSize(numCols, numRows), _hasTetromino(false), _stepIntervalInSeconds(stepIntervalInSeconds), _secondsSinceLastStep(0), _types(types)
	{
		setScale(1, float(_gridSize.y) / float(_gridSize.x));
		spawnRandomTetromino();
	}

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
		for (int i = 0; i < _gridSize.x; i++) {
			for (int j = 0; j < _gridSize.y; j++)
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
		BoardDrawStates boardStates(getScale(), _gridSize, states);

		for (std::size_t i = 0; i < _blocks.size(); i++)
			_blocks[i].drawOnBoard(target, boardStates);

		if (_hasTetromino) 
			_tetromino.drawOnBoard(target, boardStates);
	}
};

struct Scene : public sb::Drawable {
	sb::DrawBatch batch = sb::DrawBatch(2048);
	Board board = Board(10, 15, 1, { 'i', 'j' });
	
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
