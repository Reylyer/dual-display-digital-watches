#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
Adafruit_SSD1306 displayp(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

short int hour = 0;
short int minute = 0;
short int done = 0;
short int date = 1;
short int month = 1;
short int year[4] = {0,0,0,0};
short int pointer_tahun = 0;
short int end_of_the_month = 0;


void setup()   {                
  Serial.begin(9600);


  //display kiri display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3D);
  display.display();
  delay(2000);

    // Clear the buffer.
  display.clearDisplay();
  display.display();


  //display kanan displayp
  displayp.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  displayp.display();
  delay(2000);

    // Clear the buffer.
  displayp.clearDisplay();
  displayp.display();
}


void loop() {  
  if(done < 5){
    setTime();
  } else if(done == 5){
    if(digitalRead(6) == HIGH){
      done = 0;
      delay(200);
    } else if(digitalRead(6) == LOW){
    showTime();
    }
  } 
  
  Serial.println(done);
}

void setTime(){

  
  //display kiri
  // piindah ke bawah display.clearDisplay();
  if(done == 0){
    display.setCursor(5,0);
    display.setTextSize(1);
    display.println("Set hour:");
  } else if(done == 2){
    display.setCursor(5,0);
    display.setTextSize(1);
    display.println("Set minute:");
  }
  
  display.setCursor(5,20);
  display.setTextSize(4);
  display.setTextColor(WHITE);
  
  if(hour < 10){
    display.print("0");
  }
  display.print(hour);
  display.print(":");
  
  if(minute < 10){
    display.print("0");
  }
  display.print(minute);
  display.display();
  display.clearDisplay();
  
  

  //display kanan
  if(done == 2){
    displayp.setCursor(5,0);
    displayp.setTextSize(1);
    displayp.println("Set date:");
  } else if(done == 3){
    displayp.setCursor(5,0);
    displayp.setTextSize(1);
    displayp.println("Set month:");
  } else if(done == 4){
    displayp.setCursor(5,0);
    displayp.setTextSize(1);
    displayp.println("Set year:");
  }
  
   //need test
  displayp.setCursor(5,20);
  displayp.setTextSize(4);
  displayp.setTextColor(WHITE);

  if(date < 10){
    displayp.print("0");  
  }
  displayp.print(date + "/");
  
  if(month < 10){
    displayp.print("0");
  }
  displayp.print(month + "/" + *year);
  displayp.display();
  displayp.clearDisplay();


    // jam
  if(done == 0){
    if(digitalRead(8) == HIGH){
      if(hour < 24){
       hour++;
       delay(200);
      }
    } else if(digitalRead(9) == HIGH){
      if(hour > 0){
        hour--;
        delay(200);
      }
    }
    
    if(digitalRead(6) == HIGH){
      done++;
      delay(200);
    }
    
    // menit
  } if(done == 1){
    if(digitalRead(8) == HIGH){
      if(minute <60){
       minute++;
       delay(200);
      }
    } else if(digitalRead(9) == HIGH){
      if(minute > 0){
        minute--;
        delay(200);
      }
    }
    
    if(digitalRead(6) == HIGH){
      done++;
      delay(200);
    }
    
    // tanggal
  } if(done == 2){
    if(digitalRead(8) == HIGH){
      date++;
      delay(200);
    } else if(digitalRead(9) == HIGH){
      date--;
      delay(200);
    }

    if(digitalRead(6) == HIGH){
      done++;
      delay(200);
    }
    
    // bulan
  } if(done == 3){
    if(digitalRead(8) == HIGH){
      month++;
      delay(200);
    } else if(digitalRead(9) == HIGH){
      month--;
      delay(200);
    }

    if(digitalRead(6) == HIGH){
      done++;
      delay(200);
    }

    // tahun
  } if(done == 4){
    if(digitalRead(8) == HIGH){
      year[pointer_tahun] = (year[pointer_tahun] + 1)%10;
      delay(200);
    } else if(digitalRead(9) == HIGH){
      year[pointer_tahun] = (year[pointer_tahun] - 1)%10;
      delay(200);
    }
    
    if(digitalRead(6) == HIGH){
      end_of_the_month = get_end_of_the_month(month, year);
      done++;
      delay(200);
    }
    
  }
}

int get_end_of_the_month(int month, short int *year){
  
  switch(month){
    case 2:
      if(year[2]*10 + year[3] % 4 == 0){
        end_of_the_month = 29;
      } else {
        end_of_the_month = 28;
      }
      break;
    case 4:
    case 6:
    case 9:
    case 11:
      end_of_the_month = 30;
      break;
    default:
      end_of_the_month = 31;
      
  }
  
}

void showTime(){
  if(hour == 23 && minute == 60){
    hour = 0;
    minute = 0;
    date++;
    if(date == end_of_the_month + 1){
      date = 1;
      month++;
      end_of_the_month = get_end_of_the_month(month, year);
      if(month == 13){
        month = 1;
        year[3]++;
        for(int i = 3; i >= 0; i--){
          if(year[i] == 10){
            year[i] = 0;
            year[i+1]++;
          }
        }
      }
    }
  }else if(minute == 60 && hour != 23){
    hour++;
    minute = 0;
  }

   //display kiri
  display.setCursor(5,0);
  display.setTextSize(1);
  display.println("Time:");
  
  display.setCursor(5,20);
  display.setTextSize(4);
  display.setTextColor(WHITE);
  if(hour < 10){
    display.print("0");
  }
  display.print(hour);
  display.print(":");
  if(minute < 10){
    display.print("0");
  }
  display.print(minute);
  display.display();
  display.clearDisplay();

   //display kanan
  displayp.setCursor(5,20);
  displayp.setTextSize(4);
  displayp.setTextColor(WHITE);

  if(date < 10){
    displayp.print("0");  
  }
  displayp.print(date + ":");
  
  if(month < 10){
    displayp.print("0");
  }
  displayp.print(month + ":" + *year);
  displayp.display();
  displayp.clearDisplay();

  
  delay(60000);
  minute++;
}
