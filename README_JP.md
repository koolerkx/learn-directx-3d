## DirectX11 テンプレートプロジェクト

* **プロジェクト概要**

  * DirectX11 ベースの完全な 2D ゲーム開発フレームワーク
  * SHIFT-JIS エンコーディングと日本語コメント対応

* **言語オプション**

  * English（デフォルト）
  * [日本語](README_JP.md)

---

### 主な機能

1. **コアグラフィックスシステム**

   * DirectX11 Device および Context 管理
   * 変換対応 2D スプライト描画
   * フレームベースのスプライトシートアニメーション
   * テクスチャロードとリソース管理
   * HLSL シェーダーのコンパイルと管理
   * 基本的なポリゴン描画

2. **入力処理**

   * キーボード：押下、解放、トリガー検出
   * マウス：位置およびボタン状態のトラッキング
   * XInput ゲームパッド：アナログスティックと振動
   * カスタムキー割り当てシステム

3. **開発ユーティリティ**

   * 高精度タイミングによる安定フレームレート制御
   * デバッグテキスト描画（SHIFT-JIS フォント対応）
   * 強化デバッグ出力ストリーミング
   * FPS モニタリングとパフォーマンス計測

---

### プロジェクト構成

```text
learn-directX/
├── README.md                    # 本ドキュメント
├── TECHNICAL_REFERENCE.md       # API ドキュメントと例
├── DirectX.sln                  # Visual Studio ソリューション
├── DirectX/                     # ソースコード本体
│   ├── DirectX.vcxproj          # プロジェクト設定
│   ├── main.cpp                 # エントリポイントとゲームループ
│   ├── direct3d/                # グラフィックスコア
│   │   ├── direct3d.h/.cpp      # Device & Context 管理
│   │   ├── shader.h/.cpp        # シェーダーコンパイルと管理
│   │   ├── sprite.h/.cpp        # スプライト描画とアニメーション
│   │   ├── texture.h/.cpp       # テクスチャロード
│   │   └── polygon.h/.cpp       # ポリゴン描画
│   ├── game_window/             # Win32 ウィンドウ管理
│   ├── input/                   # 入力システム
│   ├── utils/                   # ユーティリティ関数
│   ├── shaders/                 # HLSL ソース
│   └── assets/                  # テクスチャとコンパイル済みシェーダー
└── doc/                         # ドキュメントとセットアップガイド
    ├── TECHNICAL_REFERENCE.md   # 英語版 API リファレンス
    ├── TECHNICAL_REFERENCE_JP.md# 日本語版 API リファレンス
    └── プロジェクト設定.md      # プロジェクト設定（日本語）
```

---

### ドキュメント

* [技術概要 (English)](doc/TECHNICAL_REFERENCE.md)
* [技術概要 (日本語)](doc/TECHNICAL_REFERENCE_JP.md)
