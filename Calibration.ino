void parseSerial()
{
	bool dotschanged = false;
	if (Serial.available() > 2)
	{
		if (Serial.read() == 'D')
		{
			switch (Serial.read())
			{
			case '1':
				switch (Serial.read())
				{
				case '+':
					divider1++;
					dotschanged = true;
					break;
				case '-':
					divider1--;
					dotschanged = true;
					break;
				}
				break;
			case '2':
				switch (Serial.read())
				{
				case '+':
					divider2++;
					dotschanged = true;
					break;
				case '-':
					divider2--;
					dotschanged = true;
					break;
				}
				break;
			case '3':
				switch (Serial.read())
				{
				case '+':
					divider3++;
					dotschanged = true;
					break;
				case '-':
					divider3--;
					dotschanged = true;
					break;
				}
				break;
			case '4':
				switch (Serial.read())
				{
				case '+':
					divider4++;
					dotschanged = true;
					break;
				case '-':
					divider4--;
					dotschanged = true;
					break;
				}
				break;
			case '5':
				switch (Serial.read())
				{
				case '+':
					divider5++;
					dotschanged = true;
					break;
				case '-':
					divider5--;
					dotschanged = true;
					break;
				}
				break;
			}
		}
	}

	if (dotschanged)
	{
		dots[0] = { CRGB::Red, divider1 };
		dots[1] = { CRGB::Red, divider2 - 1 };
		dots[2] = { CRGB::Green, divider2 };
		dots[3] = { CRGB::Green, divider3 - 1 };
		dots[4] = { CRGB::Blue, divider3 };
		dots[5] = { CRGB::Blue, divider4 - 1 };
		dots[6] = { CRGB::Yellow, divider4 };
		dots[7] = { CRGB::Yellow, divider5 - 1 };

		Serial.println("Strip1: " + String(dots[0].pos) + " - " + String(dots[1].pos));
		Serial.println("Strip2: " + String(dots[2].pos) + " - " + String(dots[3].pos));
		Serial.println("Strip3: " + String(dots[4].pos) + " - " + String(dots[5].pos));
		Serial.println("Strip4: " + String(dots[6].pos) + " - " + String(dots[7].pos));
		Serial.println();
		Serial.println();
	}

	while (Serial.available()) { Serial.read(); }
}