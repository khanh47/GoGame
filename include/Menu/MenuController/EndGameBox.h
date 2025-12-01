#pragma once

class EndGameBox {
public:
	EndGameBox() = default;
	~EndGameBox() = default;

	void open(const int &scorePlayer1, const int &scorePlayer2);
	void close();

	bool isOpen() const { return _open; }

	void update();
	void render() const;
	void handleInput();

private:
	bool _open = false;
	int _scorePlayer1;
	int _scorePlayer2;
};
