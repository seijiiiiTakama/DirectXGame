#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "ImGuiManager.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete player_;
	//delete enemys_;
	delete model_;
	delete debugCamera_;
	delete modelSkydome_;
	delete skydome_;
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("sample.png");
	// レティクルのテクスチャ
	textureReticle_ = TextureManager::Load("target.png");

	// 3Dモデルの生成
	model_ = Model::Create();

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// レールカメラの初期化
	railCamera_ = new RailCamera();
	railCamera_->Initialize({0, 0, 0}, {0, 0, 0});

	// 自キャラの生成
	player_ = new Player();
	// 自キャラとレールカメラの親子関係を結ぶ
	player_->SetParent(&railCamera_->GetWorldTransform());
	// 自キャラの初期化
	Vector3 playerPosition(0, 0, 30);
	player_->Initialize(model_, textureHandle_, textureReticle_, playerPosition);
	// 敵
	LoadEnemyPopData();

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向hy法事が参照するビュープロジェクションを指定する（アドレス無し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	// 敵キャラに自キャラのアドレスを渡す
	//enemy_->SetPlayer(player_);

	// 3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_ = new Skydome();
	// 天球の初期化
	skydome_->Initialize(modelSkydome_, {0, 0, 0});

}

void GameScene::Update() {
	
	// 自キャラの更新
	player_->Update(viewProjection_);

	// デスフラグの立った敵を削除
	enemys_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});
	// 敵キャラの更新
	for (Enemy* enemy : enemys_) {
		enemy->Update();
	}
	UpdateEnemyPopCommands();


	// デスフラグの立った弾を削除
	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	// 弾更新
	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}

	#ifdef _DEBUG
	if (input_->TriggerKey(DIK_Z)) {
		isDebugCameraActive_ = true;
	}
	#endif

	// カメラの処理
	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		// ビュー行列とプロジェクション行列をコピー
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	}

	// 弾の当たり判定
	CheckAllCollisions();

	// 天球の更新
	skydome_->Update();

	// レールカメラの更新
	railCamera_->Update();
	
	//AddEnemy();
}

void GameScene::CheckAllCollisions() {

	// 判定対象AとBの座標
	Vector3 posA, posB;

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = bullets_;

	const std::list<Enemy*>& enemys = enemys_;

#pragma region 自キャラと敵弾の当たり判定
	// 自キャラの座標
	posA = player_->GetWorldPosition();
	// 自キャラと敵弾全ての当たり判定
	for (EnemyBullet* bullet : enemyBullets) {
		// 敵弾の座標
		posB = bullet->GetWorldPosition();
		// 座標AとBの距離を求める
		// 弾と弾の交差判定
		if ((posB.x - posA.x) * (posB.x - posA.x) +
			(posB.y - posA.y) * (posB.y - posA.y) +
		    (posB.z - posA.z) * (posB.z - posA.z) <=
		    (1 + 1) * (1 + 1)) {
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			// 敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
	for (Enemy* enemy : enemys) {
		// 敵キャラの座標
		posA = enemy->GetWorldPosition();
		// 敵キャラと自弾全ての当たり判定
		for (PlayerBullet* bullet : playerBullets) {
			// 自弾の座標
			posB = bullet->GetWorldPosition();
			// 座標AとBの距離を求める
			// 弾と弾の交差判定
			if ((posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) +
			        (posB.z - posA.z) * (posB.z - posA.z) <=
			    (1 + 1) * (1 + 1)) {
				// 敵キャラの衝突時コールバックを呼び出す
				enemy->OnCollision();
				// 自弾の衝突時コールバックを呼び出す
				bullet->OnCollision();
			}
		}
	}
#pragma endregion

#pragma region 自弾と敵弾の当たり判定
	for (PlayerBullet* bullet1 : playerBullets) {
		// 自キャラの座標
		posA = bullet1->GetWorldPosition();
		// 自キャラと敵弾全ての当たり判定
		for (EnemyBullet* bullet2 : enemyBullets) {
			// 敵弾の座標
			posB = bullet2->GetWorldPosition();
			// 座標AとBの距離を求める
			// 弾と弾の交差判定
			if ((posB.x - posA.x) * (posB.x - posA.x) +
				(posB.y - posA.y) * (posB.y - posA.y) +
			    (posB.z - posA.z) * (posB.z - posA.z) <=
			    (1 + 1) * (1 + 1)) {
				// 自キャラの衝突時コールバックを呼び出す
				bullet1->OnCollision();
				// 敵弾の衝突時コールバックを呼び出す
				bullet2->OnCollision();
			}
		}
	}
#pragma endregion

	
}

void GameScene::AddEnemy(Vector3 pos) { 

	// 弾の生成
	Enemy* newEnemy = new Enemy();
	// 敵の速度
	const float kEnemySpeed = -0.1f;
	Vector3 velocity(kEnemySpeed, kEnemySpeed, kEnemySpeed);
	// 敵の初期化
	newEnemy->Initialize(model_, pos, velocity);

	// 敵キャラにゲームシーンを移す
	newEnemy->SetGameScene(this);
	// 敵に自キャラのアドレスを渡す
	newEnemy->SetPlayer(player_);

	enemys_.push_back(newEnemy);
}

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	// リストに登録する
	bullets_.push_back(enemyBullet);
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 天球の描画
	skydome_->Draw(viewProjection_);
	// 自キャラの描画
	player_->Draw(viewProjection_);
	// 敵キャラの描画
	for (Enemy* enemy : enemys_) {
		enemy->Draw(viewProjection_);
	}
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	player_->DrawUI();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::LoadEnemyPopData() {

	// ファイルを開く
	std::ifstream file;
	file.open(
	    "Resources/enemyPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void GameScene::UpdateEnemyPopCommands() {
	// 待機処理
	if (waitFlag_) {
		waitTime_--;
		if (waitTime_ <= 0) {
			// 待機完了
			waitFlag_ = false;
		}
		return;
	}

	// 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		// ,区切りで行の戦闘文字列を取得
		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 敵を発生させる
			AddEnemy(Vector3(x,y,z));

		}
		// WAITコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			// 待機開始
			waitFlag_ = true;
			waitTime_ = waitTime;

			// コマンドループを抜ける
			break;
		}
	}
}