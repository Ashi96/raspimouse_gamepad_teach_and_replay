# raspimouse_gamepad_training_replay
デモ用

## 操作方法

### トレーニング

* ロボットの前ボタンを長押しすると左横のLEDが点灯します。
この状態ではロボットがゲームパッドの操作を受け付け、
センサとモータの出力をeventというトピックに記録するようになります。
記録を終了するときは再び前ボタンを押して終了します。
記録中のトピックは~/.ros/内にバグファイルとしても記録されます。
ファイル名はROSのパラメータとして/current\_bag\_fileから得ることができます。