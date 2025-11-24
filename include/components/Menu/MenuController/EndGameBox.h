#pragma once

class EndGameBox {
public:
		EndGameBox(const int& scorePlayer1, const int& scorePlayer2)
		: _scorePlayer1(scorePlayer1), _scorePlayer2(scorePlayer2) {}
		~EndGameBox() = default;

		void open();
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
