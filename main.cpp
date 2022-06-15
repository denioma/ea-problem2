#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

class Member {
public:
    int id;
    int cost;
    int recruiter; // Never used, useless attribute
    std::vector<int> recruited;

    Member(int id) {
        Member::id = id;
        Member::recruiter = -1;
    };

    ~Member() {};
};

// member.recruiter is never used, useless function
void addRecruiters(std::vector<Member>& members) {
    for (const auto& member : members) {
        if (member.id == -1) continue;
        for (const auto& id : member.recruited) {
            members[id].recruiter = member.id;
        }
    }
}

std::vector<Member> members(100001, Member(-1));
std::vector<std::vector<std::vector<int>>> dp; // dp[member][is_included][total_chosen: 0, total_cost: 1]

int explore(int i, int guarded) {
    if (dp[i][guarded][0] != -1) { // If already cached, return cached solution
        return dp[i][guarded][0];
    }

    if (members[i].recruited.empty()) { // Base case: Leaf node
        dp[i][0] = { 0, 0 };
        dp[i][1] = { 1, members[i].cost };
        return guarded;
    }

    int sum = guarded; // Solution either includes or excludes the node
    for (const auto& child : members[i].recruited) { // Solve for the node's subtree
        if (guarded == 0) { 
            // Node is excluded, therefore all child nodes have to be included for edge inclusion
            sum += explore(child, 1); 
            // Sums cost of all child nodes to solution
            dp[i][guarded][1] += dp[child][1][1];
        } else {
            // Child nodes can be included or excluded, since the node is included and the edges are covered
            // Pick the minimum size solution. If at a stalemate, pick the highest cost
            int tmp1 = explore(child, 0);
            int tmp2 = explore(child, 1);
            if (tmp1 == tmp2) { // Stalemate, pick the highest cost
                dp[i][guarded][1] += std::max(dp[child][0][1], dp[child][1][1]);
            } else if (tmp1 < tmp2) { // Excluding the child gives the smallest solution
                dp[i][guarded][1] += dp[child][0][1];
            } else { // Including the child gives the smallest solution
                dp[i][guarded][1] += dp[child][1][1];
            }
            sum += std::min(tmp1, tmp2);
        }
    }

    dp[i][guarded][0] = sum; // Cache the total inclusions
    dp[i][guarded][1] += members[i].cost * guarded; // Add the node's cost if included
    return dp[i][guarded][0];
}

void investigate() {
    // Create DP table
    std::vector<std::vector<int>> tmp(2, {-1, 0});
    dp = std::vector<std::vector<std::vector<int>>>(members.size(), tmp);

    // Find minimum of members that need to be contacted
    int min = std::min(explore(0, 0), explore(0, 1));
    int max = 0;
    if (dp[0][0][0] == dp[0][1][0]) { // If including or not the first member gives the same sum, untie with cost
        max = std::max(dp[0][0][1], dp[0][1][1]);
    } else if (dp[0][0][0] == min) { // Not including the first member gives the smaller sum
        max = dp[0][0][1];
    } else { // Including the first member gives the smaller sum
        max = dp[0][1][1];
    }
    std::cout << min << " " << max << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);

    std::string line, token;
    int n;

    // While input not empty, extract line
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
