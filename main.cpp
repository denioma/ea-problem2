#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

class Member {
public:
    int id;
    int cost;
    int recruiter;
    std::vector<int> recruited;

    Member(int);
    ~Member() {};
    void setRecruiter(int);
    bool operator<(Member);
};

Member::Member(int id) {
    Member::id = id;
    Member::recruiter = -1;
}

bool Member::operator<(Member other) {
    return id < other.id;
}

void addRecruiters(std::vector<Member>& members) {
    for (auto& member : members) {
        for (auto& id : member.recruited) {
            members[id].recruiter = member.id;
        }
    }
}

void printMembers(std::vector<Member>& members) {
    for (const auto& member : members) {
        std::cout << "ID: " << member.id << "\n";
        if (member.recruiter != -1)
            std::cout << "Recruited by: " << member.recruiter << "\n";
        std::cout << "Paid: " << member.cost << "\n";
        if (member.recruited.empty()) continue;
        std::cout << "Recruited:";
        for (const auto& id : member.recruited) {
            std::cout << " " << id;
        }
        std::cout << "\n";
    }
}

std::vector<Member> members;
std::vector<bool> visited;
int min, max;

void explore(int i, int size, int cost) {
    // If all members visited and new minimum or equal found
    // Update min and max
    if (std::find(visited.begin(), visited.end(), false) == visited.end() && size <= min) {
        // Only save new higher cost or if size has decreased
        if (size < min || cost > max) max = cost;
        min = size;
        return;
    }

    if (size > min || i > (int)members.size())
        return;

    if (members[i].recruiter != -1) visited[members[i].recruiter] = true;
    visited[i] = true;
    for (const auto& v : members[i].recruited) {
        visited[v] = true;
    }

    explore(i + 1, size + 1, cost + members[i].cost);

    if (members[i].recruiter != -1) visited[members[i].recruiter] = false;
    visited[i] = false;
    for (const auto& v : members[i].recruited) {
        visited[v] = false;
    }

    explore(i + 1, size, cost);
}

void investigate() {
    min = (int) members.size();
    max = 0;
    visited = std::vector<bool>(min, false);
    explore(0, 0, 0);
    std::cout << min << " " << max <<"\n";
}

int main() {
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);

    std::string line, token;
    int n;

    while (std::getline(std::cin, line)) {
        std::stringstream stream(line);
        bool testcaseEnd = false;
        std::getline(stream, token, ' ');
        n = std::stod(token);
        if (n == -1) {
            std::sort(members.begin(), members.end());
            addRecruiters(members);
            // printMembers(members);
            investigate();
            members.clear();
        } else {
            Member member(n);
            while (std::getline(stream, token, ' ')) {
                n = std::stod(token);
                if ((testcaseEnd = stream.eof())) {
                    member.cost = n;
                } else {
                    member.recruited.push_back(n);
                }
            }
            members.push_back(member);
        }
    }

    return 0;
}
