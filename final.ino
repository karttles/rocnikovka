#include <Servo.h>
#define CMD_BUFFER_SIZE 20
#define CMD_SIZE 10

int stepx = 2;
int stepy = 3;
int diry = 6;
int dirx = 5;
int looper = 2200;
int loopery = 2575;
int pos = 0;
int inputIndex = 0;
char Data_in[CMD_BUFFER_SIZE];

Servo myservo;

class Souradnice
{
  public:
    int x, y;
};

Souradnice curPoziceL;

int tryParseSouradnice(char *input, Souradnice *souradniceToLoad)
{
    char x = 0;
    int y = 0;

    if (sscanf(input, " %c%d", &x, &y) != 2)
    {
        // Nepovedlo se to naparsovat ocekvali jsme že to parsne 2 v?ci ale parsnulo to m?n? v?cí
        return 1;
    }
    souradniceToLoad->x = x - 'a';
    souradniceToLoad->y = y - 1;
    return 0;
}

void move(Souradnice *start, Souradnice cil)
{
    while (start->x < cil.x)
    {
        doprava();
        start->x += 1;
    }
    while (start->x > cil.x)
    {
        doleva();
        start->x -= 1;
    }
    while (start->y < cil.y)
    {
        nahoru();
        start->y += 1;
    }
    while (start->y > cil.y)
    {
        dolu();
        start->y -= 1;
    }
}

void nalozit()
{
    for (pos = 115; pos >= 20; pos -= 1)
    {
        myservo.write(pos);
        delay(15);
    }
    delay(500);
    digitalWrite(diry, LOW);
    for (int i = 0; i < 300; i++)
    {
        digitalWrite(stepy, HIGH);
        delay(1);
        digitalWrite(stepy, LOW);
        delay(1);
    }
    for (pos = 20; pos <= 115; pos += 1)
    {
        myservo.write(pos);
        delay(15);
    }
    digitalWrite(diry, HIGH);
    for (int i = 0; i < 300; i++)
    {
        digitalWrite(stepy, HIGH);
        delay(1);
        digitalWrite(stepy, LOW);
        delay(1);
    }
}

void vylozit()
{
    digitalWrite(diry, LOW);
    for (int i = 0; i < 300; i++)
    {
        digitalWrite(stepy, HIGH);
        delay(1);
        digitalWrite(stepy, LOW);
        delay(1);
    }
    for (pos = 115; pos >= 20; pos -= 1)
    {
        myservo.write(pos);
        delay(15);
    }
    digitalWrite(diry, HIGH);
    for (int i = 0; i < 300; i++)
    {
        digitalWrite(stepy, HIGH);
        delay(1);
        digitalWrite(stepy, LOW);
        delay(1);
    }
    delay(500);
    for (pos = 20; pos <= 115; pos += 1)
    {
        myservo.write(pos);
        delay(15);
    }
}

void doprava()
{
    digitalWrite(dirx, HIGH);
    for (int i = 0; i < looper; i++)
    {
        digitalWrite(stepx, HIGH);
        delay(1);
        digitalWrite(stepx, LOW);
        delay(1);
    }
    return 0;
}
void doleva()
{
    digitalWrite(dirx, LOW);
    for (int i = 0; i < looper; i++)
    {
        digitalWrite(stepx, HIGH);
        delay(1);
        digitalWrite(stepx, LOW);
        delay(1);
    }
    return 0;
}
void nahoru()
{
    digitalWrite(diry, LOW);
    for (int i = 0; i < loopery; i++)
    {
        digitalWrite(stepy, HIGH);
        delay(1);
        digitalWrite(stepy, LOW);
        delay(1);
    }
    return 0;
}
void dolu()
{
    digitalWrite(diry, HIGH);
    for (int i = 0; i < loopery; i++)
    {
        digitalWrite(stepy, HIGH);
        delay(1);
        digitalWrite(stepy, LOW);
        delay(1);
    }
    return 0;
}

// move a1
int vykonej()
{
    char prikaz[CMD_SIZE];
    // a1

    sscanf(Data_in, "%s", prikaz);
    char *argumenty = Data_in + strlen(prikaz);
    if (strcmp(prikaz, "move") == 0)
    {

        Souradnice cil;
        if (tryParseSouradnice(argumenty, &cil) != 0)
        {
            Serial.print("Big oof nepovedlo se naparasovat sou?adnice\n");
        }

        move(&curPoziceL, cil);
        Serial.println("move");
        return 0;
    }
    else if (strcmp(prikaz, "nalozit") == 0)
    {
        Serial.println("Nalozit");
        nalozit();
    }
    else if (strcmp(prikaz, "vylozit") == 0)
    {
      Serial.println("Vylozit");
        vylozit();
    }
    else
    {
        Serial.println("Prikaz neznam");
        return 1;
    }
    return 0;
}

int readSerialPort()
{
    int intRead;
    while ((intRead = Serial.read()) != -1)
    {

        char charRead = (char)intRead;


        if (charRead == '\n')
        {
            // Finished laoding
            Data_in[inputIndex% CMD_BUFFER_SIZE] = '\0';
            inputIndex = 0;
            return 0;
        }
        // buffer overflow prevention
        Data_in[inputIndex % CMD_BUFFER_SIZE] = charRead;
        ++inputIndex;
    }
    return 1;
}

void setup()
{
    Serial.begin(9600);
    pinMode(stepx, OUTPUT);
    pinMode(stepy, OUTPUT);
    pinMode(diry, OUTPUT);
    pinMode(dirx, OUTPUT);
    myservo.attach(11); // pin
    curPoziceL = {.x = 0,
                  .y = 0 };
}

void loop()
{
    int dataRead = readSerialPort();
    if(dataRead != 0){
      return;
    }
    vykonej();
}
