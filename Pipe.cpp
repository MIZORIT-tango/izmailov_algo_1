#include "pipe.h"
#include "globals.h"

Pipe::Pipe() : id(0), name(""), length(0.0f), diameter(0), status(true),
isConnected(false), startStationId(-1), endStationId(-1) {
}

Pipe::Pipe(int pipeId, const std::string& n, float l, int d)
    : id(pipeId), name(n), length(l), diameter(d), status(true),
    isConnected(false), startStationId(-1), endStationId(-1) {
}

double Pipe::getCapacity() const {
    if (status) {
        return 0.0;
    }

    auto it = PIPE_DIAMETERS_CAPACITY.find(diameter);
    if (it != PIPE_DIAMETERS_CAPACITY.end()) {
        return it->second;
    }
    return 0.0;
}

double Pipe::getWeightForPath() const {
    if (status) {
        return std::numeric_limits<double>::infinity();
    }
    return static_cast<double>(length); 
}

void Pipe::connect(int startId, int endId) {
    isConnected = true;
    startStationId = startId;
    endStationId = endId;
}

void Pipe::disconnect() {
    isConnected = false;
    startStationId = -1;
    endStationId = -1;
}

bool Pipe::isConnectedToStation(int stationId) const {
    return isConnected && (startStationId == stationId || endStationId == stationId);
}

bool Pipe::switchStatus(int o) {
    if (o == 1) {
        status = true;
        return true;
    }
    else if (o == 2) {
        status = false;
        return true;
    }
    else {
        return status;
    }
}

std::ostream& operator<<(std::ostream& out, const Pipe& p) {
    if (&out == &std::cout) {
        out << "ID: " << p.id << "\n";
        out << "Name: " << p.name << "\n";
        out << "Length: " << p.length << "\n";
        out << "Diameter: " << p.diameter << "\n";
        out << "Status: " << (p.status ? "Under renovation" : "Working") << "\n";
        out << "Connected: " << (p.isConnected ? "Yes" : "No") << "\n";
        if (p.isConnected) {
            out << "Start Station ID: " << p.startStationId << "\n";
            out << "End Station ID: " << p.endStationId << "\n";
        }
        out << "\n";
    }
    else {
        out << p.id << "\n";
        out << p.name << "\n";
        out << p.length << "\n";
        out << p.diameter << "\n";
        out << p.status << "\n";
        out << p.isConnected << "\n";
        out << p.startStationId << "\n";
        out << p.endStationId << "\n";
    }
    return out;
}

std::istream& operator>>(std::istream& in, Pipe& p) {
    in >> p.id;
    in.ignore();
    std::getline(in, p.name);
    in >> p.length;
    in >> p.diameter;
    in >> p.status;
    in >> p.isConnected;
    in >> p.startStationId;
    in >> p.endStationId;
    return in;
}