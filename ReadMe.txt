Commandoについて

1.目的
  普段アルゴリズムを走らせてその出力にしか使っていないようなコマンドプロンプトの上でアクションゲームを動かしたらどうなるかと思い立ちこのゲームを制作した次第である.

2.概要
  Commandoはコマンドプロンプトで動くシューティングゲームである.
  コマンドプロンプトでの自由な文字描画にはWindows Console APIを使用している.
  描画関連はWinAPI.cppを参照されたい. (雑なコードですが...)

3.仕様
  本ゲームの仕様を以下に示す.
    初期残機 := 3
    敵を倒すとScore+1
    敵弾に当たると残機-1
    残機0でGame Over
    

4.操作方法
  初めに表示される"GAME OVER"画面にて"Space" Keyを押すとスタート,'Delete' Keyを押すと終了.
  ゲームプレイ中の自機の操作は以下の通り
    'H' Key:左へ移動
    'L' Key:右へ移動
    'K' Key:上へ移動
    'J' Key:下へ移動
　　'A' Key:弾発射

5.その他
  コンパイル時は, 全てのcpp Fileをコンパイルして, リンクする.
  例: g++ Main.cpp WinAPI.cpp ... EnemyBeam.cpp