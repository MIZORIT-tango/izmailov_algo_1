#ifndef PIPE_H
#define PIPE_H

#include <string>
#include <iostream>

class Pipe {
private:
    int id;
    std::string name;
    float length;
    int diameter;
    bool status;
    bool isConnected;
    int startStationId;
    int endStationId;

public:
    Pipe();
    Pipe(int pipeId, const std::string& n, float l, int d);

    int getId() const { return id; }
    std::string getName() const { return name; }
    float getLength() const { return length; }
    int getDiameter() const { return diameter; }
    bool getStatus() const { return status; }
    bool getIsConnected() const { return isConnected; }
    int getStartStationId() const { return startStationId; }
    int getEndStationId() const { return endStationId; }

    void setName(const std::string& n) { name = n; }
    void setLength(float l) { length = l; }
    void setDiameter(int d) { diameter = d; }
    void setStatus(bool s) { status = s; }

    void connect(int startId, int endId);
    void disconnect();
    bool isConnectedToStation(int stationId) const;

    bool switchStatus(int o);

    friend std::ostream& operator<<(std::ostream& out, const Pipe& p);
    friend std::istream& operator>>(std::istream& in, Pipe& p);
};

#endif