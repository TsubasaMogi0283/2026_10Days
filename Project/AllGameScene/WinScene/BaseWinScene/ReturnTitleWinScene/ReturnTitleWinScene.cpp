#include "ReturnTitleWinScene.h"

#include <numbers>
#include <imgui.h>

#include "Input.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "LevelDataManager.h"
#include "GlobalVariables.h"
#include "Audio.h"

#include "Easing.h"
#include "VectorCalculation.h"
#include "WinScene/WinScene.h"

ReturnTitleWinScene::ReturnTitleWinScene() {
	//インスタンスの取得
	//入力クラス
	input_ = Kamaboko::Input::GetInstance();
	//テクスチャ管理クラス
	textureManager_ = Kamaboko::TextureManager::GetInstance();
	//レベルデータ管理クラス
	levelDataManager_ = Kamaboko::LevelDataManager::GetInstance();
	//グローバル変数クラス
	globalVariables_ = Kamaboko::GlobalVariables::GetInstance();
	//オーディオ
	audio_ = Kamaboko::Audio::GetInstance();
}


void ReturnTitleWinScene::Initialize(){

	//白背景
	uint32_t whiteTexturehandle = Kamaboko::TextureManager::GetInstance()->Load("Resources/Sprite/Back/White.png");
	//生成
	whiteFadeSprite_.reset(Kamaboko::Sprite::Create(whiteTexturehandle, INITIAL_SPRITE_POSITION_));
	//透明度の設定
	transparency_ = 0.0f;
	whiteFadeSprite_->SetTransparency(transparency_);

	//決定音
	sweepUpSEHandle_ = audio_->Load("Resources/Audio/SE/SweepUp.wav");
	audio_->Play(sweepUpSEHandle_, false);
	audio_->ChangeVolume(sweepUpSEHandle_, sweepUpSEVolume_);
}

void ReturnTitleWinScene::Update(WinScene* winScene){
	//加速
	cameraAcceleration_.z = 0.05f;
	cameraVelocity_ = VectorCalculation::Add(cameraVelocity_, cameraAcceleration_);
	//本体に勝利シーンに設定
	winScene->SetCameraVelocity(cameraVelocity_);

	//透明度の設定。どんどん白くなっていく
	transparency_ += INCREASE_TRANSPARENCY_VALUE_;
	whiteFadeSprite_->SetTransparency(transparency_);

	//動く時間を加算
	moveTime_ += DELTA_TIME_;

	//BGMの音量を下げていく
	winScene->BgmVolumeDown(BGM_VOLUME_DECREASE_VALUE_);

	//動く時間が終わったら処理終了
	if (moveTime_>MOVE_END_TIME_) {
		//処理終了を示す
		winScene->SetIsEnd();
		return;
	}

#ifdef _DEBUG
	//ImGui表示用
	DisplayImGui();
#endif // _DEBUG

}

void ReturnTitleWinScene::DrawSprite(){
	//白フェードの設定
	whiteFadeSprite_->Draw();

}

void ReturnTitleWinScene::DisplayImGui(){
	ImGui::Begin("タイトルへ戻る(勝利シーン)");
	ImGui::InputFloat("動く時間", &moveTime_);

	ImGui::End();
}
