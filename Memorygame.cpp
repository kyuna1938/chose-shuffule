#include "Memorygame.hpp"


Memorygame::Memorygame(const InitData& init) : IScene(init) {
	setCards();
	if (getData().shuffleKind == 0) {//�V���b�t���̎�ޑI��
		cards = hinduShuffle(cards, getData().roop);
	} else {
		cards = farrowShuffle(cards, getData().roop);
	}
	for (int i = 0; i < 60; i++) {//�J�[�h�̂����蔻��p�̓����Ȑ}�`�̔z�u
		RectF rect(15 + i % 15 * (pack.width() + 15), 15 + (i / 15) * (pack.height() + 15), 69, 111.64435);
		rects.push_back(rect);
	}
	for (int i = 0; i < 60; i++) {//�z�񏉊���
		gameResult[i] = true;
	}
}

void Memorygame::update() {
	if (count == 2) {//count2�̏�Ԃ�drow���Ăяo�����ƂŊԈ�����ꍇ�ł�2���Ƃ��\�����邱�Ƃ��ł���悤�ɂ������A���艟�������Ȃ̂ł����������P������
		count++;
	}
	for (int i = 0; i < rects.size(); i++) {//�N���b�N�����J�[�h��\�Ɍ�����
		if (rects[i].leftClicked() && gameResult[i] && count < 2) {
			savei[count] = i;
			gameResult[i] = false;
			cards[i].flip();
			count++;//�J�����J�[�h�̖������J�E���g����
			break;
		}
	}
	
	if (count == 3) {
		
		if (cards[savei[0]].rank == cards[savei[1]].rank) {//�J�[�h���ꏏ���ǂ���
			
		} else {
			
			gameResult[savei[0]] = true;
			gameResult[savei[1]] = true;
			System::Sleep(2000);//�J�[�h�\������
			cards[savei[0]].flip();
			cards[savei[1]].flip();

		}
		count = 0;

	}
	if (MouseR.pressed()) {//title�ɖ߂�
		changeScene(State::Title, 0.3s);
	}
}

void Memorygame::draw() const {
	for (int i = 0; i < 60; i++) {//�J�[�h�̕`��
		const Vec2 center(15 + i % 15 * (pack.width() + 15), 15 + (i / 15) * (pack.height() + 15));
		pack(cards[i]).draw(center);
	}

}

Array<PlayingCard::Card> Memorygame::hinduShuffle(Array<PlayingCard::Card> cards, int roop) {
	int count;
	int now;
	std::normal_distribution<> dist(10, 5 / 3);
	Array<PlayingCard::Card> afterCards = cards.slice(0);
	for (int i = 0; i < roop; i++) {
		cards = afterCards.slice(0);
		count = cards.size();
		now = 0;
		while (count > 0) {
			int stack = (int)dist(engine);
			for (int i = stack; i > 0; i--) {
				if (now + i - 1 >= afterCards.size()) {
					continue;
				}
				afterCards[count - 1] = cards[now + i - 1];
				count--;
				if (count <= 0) {
					break;
				}
			}
			now += stack;
		}
	}
	return afterCards;

}
Array<PlayingCard::Card> Memorygame::farrowShuffle(Array<PlayingCard::Card> cards, int roop) {
	int acount;
	int bcount;
	int now;
	Array<PlayingCard::Card> afterCards = cards.slice(0);
	std::binomial_distribution<> dist(cards.size(), 0.5);
	for (int j = 0; j < roop; j++) {
		now = cards.size();
		int div = dist(engine);
		Array<PlayingCard::Card> a;
		Array<PlayingCard::Card> b;
		for (int i = 0; i < div; i++) {
			a.push_back(afterCards[i]);
		}
		for (int i = 0; i < cards.size() - div; i++) {
			b.push_back(afterCards[i + div]);
		}
		acount = div;
		bcount = cards.size() - div;

		while (acount > 0 || bcount > 0) {
			if (acount == 0) {
				for (int i = 0; i < bcount; i++) {
					afterCards[i] = b[i];
				}
				break;
			}
			if (bcount == 0) {
				for (int i = 0; i < acount; i++) {
					afterCards[i] = a[i];
				}
				break;
			}
			double x = acount / (double)(acount + bcount);
			if (x >= Random(0.0, 1.0)) {
				afterCards[now - 1] = a[acount - 1];
				now--;
				acount--;
			}
			else {
				afterCards[now - 1] = b[bcount - 1];
				now--;
				bcount--;
			}
		}
	}
	return afterCards;

}

void Memorygame::setCards() {//�J�[�h�̏����Z�b�g
	cards.clear();
	cardData = getData().cards;
	for (int i = 0; i < 60; i++) {
		if (cardData[i].y == 0) {
			cards.push_back(PlayingCard::Card(PlayingCard::Suit::Spade, cardData[i].x, false));
		}
		else {
			cards.push_back(PlayingCard::Card(PlayingCard::Suit::Diamond, cardData[i].x, false));
		}
	}

}