﻿# include <Siv3D.hpp>

void Main()
{
	// 1 つのブロックのサイズ | Size of a single block
	constexpr Size BrickSize{ 40, 20 };

	// ボールの速さ（ピクセル / 秒） | Ball speed (pixels / second)
	constexpr double BallSpeedPerSec = 480.0;

	// ボールの速度 | Ball velocity
	Vec2 ballVelocity{ 0, -BallSpeedPerSec };

	// ボール | Ball
	Circle ball{ 400, 400, 8 };

	// プレイヤーの絵文字テクスチャ
	const Texture emoji{ U"🥺"_emoji };

	// ブロックの配列 | Array of bricks
	Array<Rect> bricks;

	for (int32 y = 0; y < 5; ++y)
	{
		for (int32 x = 0; x < (Scene::Width() / BrickSize.x); ++x)
		{
			bricks << Rect{ (x * BrickSize.x), (60 + y * BrickSize.y), BrickSize };
		}
	}

	int paddlex = 8;

	double sp = 0.0;

	int combo = 0;

	int score = 0;


	const Font font{ FontMethod::MSDF, 48 };


	while (System::Update())
	{
		font(U"score : {}"_fmt(score)).draw(32, 0, 0, ColorF{ 1.0, 1.0, 1.0 });
		if (combo > 1) {
			font(U"{} combo!"_fmt(combo)).draw(32, 300, 400, ColorF{ 1.0, 1.0, 1.0 });
		}

		// パドル | Paddle
		const Rect paddle{ Arg::center(Cursor::Pos().x, 500), paddlex, 10 };

		// ボールを移動させる | Move the ball
		ball.moveBy(ballVelocity * Scene::DeltaTime());

		// ブロックを順にチェックする | Check bricks in sequence
		for (auto it = bricks.begin(); it != bricks.end(); ++it)
		{
			// ブロックとボールが交差していたら | If block and ball intersect
			if (it->intersects(ball))
			{
				// ブロックの上辺、または底辺と交差していたら | If ball intersects with top or bottom of the block
				if (it->bottom().intersects(ball) || it->top().intersects(ball))
				{
					// ボールの速度の Y 成分の符号を反転する | Reverse the sign of the Y component of the ball's velocity
					ballVelocity.y *= -1;
				}
				else // ブロックの左辺または右辺と交差していたら
				{
					// ボールの速度の X 成分の符号を反転する | Reverse the sign of the X component of the ball's velocity
					ballVelocity.x *= -1;
				}

				// ブロックを配列から削除する（イテレータは無効になる） | Remove the block from the array (the iterator becomes invalid)
				bricks.erase(it);

				paddlex = paddlex + 2;

				sp = sp + 100;

				combo++;

				if (combo < 2)
				{
					score = score + 100;
				}
				else if (combo < 5)
				{
					score = score + 100 * 1.1;
				}
				else if (combo < 7)
				{
					score = score + 100 * 1.2;
				}
				else if (combo < 10)
				{
					score = score + 100 * 1.3;
				}
				else if (combo <15)
				{
					score = score + 100 * 1.5;
				}
				else if (combo < 25)
				{
					score = score + 100 * 1.7;
				}
				else if (combo < 40)
				{
					score = score + 100 * 2.0;
				}
				else if (combo > 40)
				{
					score = score + 100 * 3.0;;
				}

				// これ以上チェックしない | Do not check any more
				break;
			}
		}

		// 天井にぶつかったら | If the ball hits the ceiling
		if ((ball.y < 0) && (ballVelocity.y < 0))
		{
			// ボールの速度の Y 成分の符号を反転する | Reverse the sign of the Y component of the ball's velocity
			ballVelocity.y *= -1;
		}

		// 左右の壁にぶつかったら | If the ball hits the left or right wall
		if (((ball.x < 0) && (ballVelocity.x < 0))
			|| ((Scene::Width() < ball.x) && (0 < ballVelocity.x)))
		{
			// ボールの速度の X 成分の符号を反転する | Reverse the sign of the X component of the ball's velocity
			ballVelocity.x *= -1;
		}

		// パドルにあたったら | If the ball hits the left or right wall
		if ((0 < ballVelocity.y) && paddle.intersects(ball))
		{
			// パドルの中心からの距離に応じてはね返る方向（速度ベクトル）を変える | Change the direction (velocity vector) of the ball depending on the distance from the center of the paddle
			ballVelocity = Vec2{ (ball.x - paddle.center().x) * 10, -ballVelocity.y }.setLength(BallSpeedPerSec);
			combo = 0;
		}

		// すべてのブロックを描画する | Draw all the bricks
		for (const auto& brick : bricks)
		{
			// ブロックの Y 座標に応じて色を変える | Change the color of the brick depending on the Y coordinate
			brick.stretched(-1).draw(HSV{ brick.y - 40 });
		}

		// マウスカーソルを非表示にする | Hide the mouse cursor
		Cursor::RequestStyle(CursorStyle::Hidden);

		// ボールを描く | Draw the ball
		ball.draw();

		// パドルを描く | Draw the paddle
		paddle.rounded(3).draw();

		emoji.scaled(0.2).drawAt(ball.x, ball.y);

		Rect(20, 40, 400, 100);

		//emoji.rotated(100_deg).drawAt(100, 100);

	}
}
