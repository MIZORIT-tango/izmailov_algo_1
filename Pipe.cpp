#include "pipe.h"

Pipe::Pipe() : id(0), name(""), length(0.0f), diameter(0), status(true) {}

Pipe::Pipe(int pipeId, const std::string& n, float l, int d)
    : id(pipeId), name(n), length(l), diameter(d), status(true) {
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
        out << "Status: " << (p.status ? "Under renovation" : "Working") << "\n\n";
    }
    else {
        out << p.id << "\n";
        out << p.name << "\n";
        out << p.length << "\n";
        out << p.diameter << "\n";
        out << p.status << "\n";
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
    return in;
}