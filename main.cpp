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
        if (member.id == -1) continue;
        for (auto& id : member.recruited) {
            members[id].recruiter = member.id;
        }
    }
}

std::vector<Member> members(100001, Member(-1));
std::vector<bool> visited;
std::vector<std::vector<std::vector<int>>> dp;

int explore(int i, int guarded) {
    if (members[i].recruited.empty()) { // Leaf
        dp[i][1][1] = members[i].cost;
        return guarded;
    } else if (dp[i][guarded][0] != -1) { // If already cached
        return dp[i][guarded][0];
    }

    int sum = 0;
    for (const auto& id : members[i].recruited) {
        if (guarded == 0) { // Must select all child nodes for edge inclusion
            sum += explore(id, 1);
            // Adds label of all child nodes to cost
            dp[i][guarded][1] += dp[id][1][1];
        } else {
            int tmp1 = explore(id, 0);
            int tmp2 = explore(id, 1);
            if (tmp2 <= tmp1) {
                dp[i][guarded][1] += members[id].cost;
            }
            sum += std::min(explore(id, 0), explore(id, 1));
        }
    }
    dp[i][guarded][0] = sum + guarded;
    if (guarded) dp[i][guarded][1] += members[i].cost;
    return dp[i][guarded][0];
}

void investigate() {
    visited = std::vector<bool>(members.size(), false);
    std::vector<std::vector<int>> tmp(2, {-1, 0});
    dp = std::vector<std::vector<std::vector<int>>>(members.size(), tmp);
    int min = std::min(explore(0, 0), explore(0, 1));
    int max = 0;
    if (dp[0][0][0] == min) {
        if (dp[0][1][0] == min)
            max = std::max(dp[0][0][1], dp[0][1][1]);
        else max = dp[0][0][1];
    } else {
        max = dp[0][1][1];
    }
    std::cout << min << " " << max << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);

    std::string line, token;
    int n;

    while (std::getline(std::cin, line)) {
        std::stringstream stream(line);
        std::getline(stream, token, ' ');
        n = std::stoi(token);
        if (n == -1) { // End of testcase input
            addRecruiters(members);
            investigate();
            members = std::vector<Member>(100001, Member(-1));
        } else { // Testcase input
            Member member(n);
            while (std::getline(stream, token, ' ')) {
                n = std::stoi(token);
                member.recruited.push_back(n);
            }
            member.recruited.pop_back();
            member.cost = n;
            members[member.id] = member;
        }
    }

    return 0;
}
