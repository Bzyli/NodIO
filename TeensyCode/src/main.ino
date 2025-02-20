#include <Arduino.h>
#include <Audio.h>

#define MAX_NODES 10
#define MAX_CONNS 10

typedef struct {
    int id;
    char type[10]; 
    float param;    
    void *audioObj; 
} Node;

typedef struct {
    int from;
    int to;
} Connection;

Node nodes[MAX_NODES];
Connection conns[MAX_CONNS];
AudioConnection* audioConns[MAX_CONNS];
int nodeCount = 0, connCount = 0;

// Teensy Audio Objects
AudioInputI2S  audioInput;
AudioOutputI2S audioOutput;
AudioStream      *effects[MAX_NODES] = {nullptr};  // Handles both lowcut and delay

// Audio Control
AudioControlSGTL5000 audioShield;

char *skipSpaces(char *s) {
    while (*s && isspace((unsigned char)*s)) s++;
    return s;
}

void resetAudioGraph() {
    for (int i = 0; i < connCount; i++) {
        delete audioConns[i];
        audioConns[i] = nullptr;
    }
    connCount = 0;

    for (int i = 0; i < MAX_NODES; i++) {
        if (effects[i]) {
            delete effects[i];
            effects[i] = nullptr;
        }
    }
}

void parseConfig(char *config) {
    resetAudioGraph();
    nodeCount = 0;
    connCount = 0;

    char *nodesStart = strstr(config, "NODES[");
    if (!nodesStart) return;
    nodesStart += strlen("NODES[");

    char *nodesEnd = strchr(nodesStart, ']');
    if (!nodesEnd) return;
    *nodesEnd = '\0';

    char *token = strtok(nodesStart, ",");
    while (token && nodeCount < MAX_NODES) {
        token = skipSpaces(token);
        Node n;
        int items = sscanf(token, "%d %9s %f", &n.id, n.type, &n.param);
        if (items >= 2) {
            if (items == 2) n.param = 0.0f;

            if (strcmp(n.type, "in") == 0) {
                n.audioObj = &audioInput;
            }
            else if (strcmp(n.type, "out") == 0) {
                n.audioObj = &audioOutput;
            }
            else if (strcmp(n.type, "lowcut") == 0) {
                effects[nodeCount] = new AudioFilterBiquad();
                ((AudioFilterBiquad*)effects[nodeCount])->setHighpass(0, (int) n.param, 0.7);
                n.audioObj = effects[nodeCount];
            }
            else if (strcmp(n.type, "delay") == 0) {
                effects[nodeCount] = new AudioEffectDelay();
                ((AudioEffectDelay*)effects[nodeCount])->delay(0,(int) n.param); // Apply delay in milliseconds
                n.audioObj = effects[nodeCount];
            } else if (strcmp(n.type, "volume") == 0) {
                effects[nodeCount] = new AudioAmplifier();
                ((AudioAmplifier*)effects[nodeCount])->gain(n.param);
                Serial.print("got volume");
                Serial.println(n.param / 1.0);
                n.audioObj = effects[nodeCount];
            } else if (strcmp(n.type, "reverb") == 0) {
                effects[nodeCount] = new AudioEffectReverb();
                ((AudioEffectReverb*)effects[nodeCount])->reverbTime((int) n.param);
                n.audioObj = effects[nodeCount];
            }

            nodes[nodeCount++] = n;
        }
        token = strtok(NULL, ",");
    }
    *nodesEnd = ']';

    char *connsStart = strstr(nodesEnd, "CONNS[");
    if (!connsStart) return;
    connsStart += strlen("CONNS[");

    char *connsEnd = strchr(connsStart, ']');
    if (!connsEnd) return;
    *connsEnd = '\0';

    token = strtok(connsStart, ",");
    while (token && connCount < MAX_CONNS) {
        token = skipSpaces(token);
        Connection c;
        if (sscanf(token, "%d %d", &c.from, &c.to) == 2) {
            conns[connCount++] = c;
        }
        token = strtok(NULL, ",");
    }
    *connsEnd = ']';

    for (int i = 0; i < connCount; i++) {
        Node *fromNode = nullptr;
        Node *toNode = nullptr;

        for (int j = 0; j < nodeCount; j++) {
            if (nodes[j].id == conns[i].from) fromNode = &nodes[j];
            if (nodes[j].id == conns[i].to) toNode = &nodes[j];
        }

        if (fromNode && toNode && fromNode->audioObj && toNode->audioObj) {
            int fromChannel = (strcmp(fromNode->type, "in") == 0) ? fromNode->param - 1 : 0;
            int toChannel = (strcmp(toNode->type, "out") == 0) ? toNode->param - 1 : 0;
            audioConns[i] = new AudioConnection(*(AudioStream*)fromNode->audioObj, fromChannel, *(AudioStream*)toNode->audioObj, toChannel);
        }
    }

    Serial.println("Audio graph updated.");
}

void setup() {
    Serial.begin(115200);
    while (!Serial) {}

    AudioMemory(400);
    audioShield.enable();
    audioShield.inputSelect(AUDIO_INPUT_LINEIN);
    Serial.println("Waiting for input...");
}

void loop() {
    static char inputBuffer[256];
    static int bufferIndex = 0;

    while (Serial.available()) {
        char c = Serial.read();
        if (c == '\n') {
            inputBuffer[bufferIndex] = '\0';
            Serial.println("\nReceived new config:");
            Serial.println(inputBuffer);
            parseConfig(inputBuffer);
            bufferIndex = 0;
        } else if (bufferIndex < sizeof(inputBuffer) - 1) {
            inputBuffer[bufferIndex++] = c;
        }
    }
}
