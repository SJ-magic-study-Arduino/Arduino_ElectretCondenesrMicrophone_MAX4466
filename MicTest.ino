/************************************************************
■参考
	■マイクモジュールをArduinoに繋ぎPCでProcessingを使って録音する
		https://nixeneko.hatenablog.com/entry/2016/09/14/150506
		
		■エレクトレットマイクアンプモジュール
			https://www.switch-science.com/catalog/1680/
	
	■Electret Microphone Amplifier - MAX4466 with Adjustable Gain
		https://www.adafruit.com/product/1063
		
		contents
			Using it is simple: connect GND to ground, VCC to 2.4-5VDC. For the best performance, use the "quietest" supply available (on an Arduino, this would be the 3.3V supply). 
			
	■インタラクティブアート木曜日授業記録
		https://idd-interactive-art.tumblr.com/post/145652587306/%E3%83%9E%E3%82%A4%E3%82%AF%E3%83%A2%E3%82%B8%E3%83%A5%E3%83%BC%E3%83%AB%E3%82%92%E4%BD%BF%E3%81%A3%E3%81%A6%E9%9F%B3%E9%87%8F%E3%82%92%E5%8F%96%E5%BE%97%E3%81%97%E3%81%A6%E3%81%BF%E3%82%8B/amp
	
	■様々なLEDキャンドル
		http://lecture.nakayasu.com/?p=2392
		
	■マイクモジュールmax4466を使ってみる
		https://novicengineering.com/%E3%83%9E%E3%82%A4%E3%82%AF%E3%83%A2%E3%82%B8%E3%83%A5%E3%83%BC%E3%83%ABmax4466%E3%82%92%E4%BD%BF%E3%81%A3%E3%81%A6%E3%81%BF%E3%82%8B/

************************************************************/

#define BAUD 9600
#define MIC_IN_ANALOG		0
#define LED_OUT_DIGITAL		2	// serial通信中 : 0, 1 番pin は、使用不可

enum STATE{
	STATE_ON,
	STATE_OFF,
};
STATE State = STATE_ON;

unsigned long t_from = 0;


/************************************************************
************************************************************/

/******************************
******************************/
void setup() {
	Serial.begin(BAUD);
	pinMode(LED_OUT_DIGITAL, OUTPUT);
}

/******************************
******************************/
void loop() {
	/********************
	Get value
	********************/
	int val = analogRead(MIC_IN_ANALOG);
	float volts = (float)val / 1023 * 5.0;	// 0.0 <= volts <= 3.3
	float VoiceAmp = abs(volts - 1.65);
	
	/********************
	Led Control
	********************/
	/* StateChart */
	float th_Amp = 1.0;
	float timeout = 3000;
	switch(State){
		case STATE_ON:
			if(th_Amp < VoiceAmp){
				State = STATE_OFF;
				t_from = millis();
			}
			break;
			
		case STATE_OFF:
			if(th_Amp < VoiceAmp)			t_from = millis();
			if(timeout < millis() - t_from)	State = STATE_ON;
			
			break;
	}
	
	/* Led control */
	if(State == STATE_ON)	digitalWrite(LED_OUT_DIGITAL, HIGH);
	else					digitalWrite(LED_OUT_DIGITAL, LOW);
	
	/********************
	********************/
	Serial.print(millis() / 1000 % 2 * 5.0);
    Serial.print(",");
    Serial.print(2.475);	// Amp = 1.65/2 = 0.825
    Serial.print(",");
    Serial.print(3.3);
    Serial.print(",");
	Serial.println(volts);
	
	/********************
	********************/
	delay(10);
}


