#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>
#include <regex>
#include <cassert>
#include <set>

class Domain {
public:
   Domain(std::string domain) : name_(std::move(std::string(domain.rbegin(), domain.rend()))) {}
   bool IsSubdomain(const Domain& subdomain) const
   {
      using namespace std::literals;
      std::regex r("^"s + name_ + "\\."s);
      return std::regex_search(subdomain.GetReverseName(), r);
   }

   std::string GetReverseName() const {
      return name_;
   }

   bool operator==(const Domain& other) const
   {
      return name_ == other.name_;
   }

   bool operator<(const Domain& other) const {
      return name_ < other.name_;
   }

private:
   std::string name_;
};

class DomainChecker {
public:
   template <typename Iterator>
   DomainChecker(Iterator begin, Iterator end)
   {
      while (begin != end)
      {
         domain_store_.push_back(*begin++);
      }
      std::sort(domain_store_.begin(), domain_store_.end());
      auto it = std::unique(domain_store_.begin(), domain_store_.end(), [](const auto& lhs, const auto& rhs) {
         return (lhs == rhs || lhs.IsSubdomain(rhs) || rhs.IsSubdomain(lhs));
         });
      domain_store_.erase(it, domain_store_.end());
   }

   bool IsForbidden(const Domain& domain)
   {
      if (!domain_store_.empty()) {
         auto it = std::lower_bound(domain_store_.begin(), domain_store_.end(), domain);
         return((it != domain_store_.end() && *it == domain) || (*(it == domain_store_.begin() ? it : --it)).IsSubdomain(domain));
      }
      return false;
   }

private:
   std::vector<Domain> domain_store_;
};

template <typename Number>
std::vector<Domain> ReadDomains(std::istream& in, Number count)
{
   std::vector<Domain> domain_store;
   while (count--) {
      std::string line;
      std::getline(in, line);
      domain_store.emplace_back(line);
   }
   return domain_store;
}

template <typename Number>
Number ReadNumberOnLine(std::istream& input)
{
   Number num;
   std::string line;
   std::getline(input, line);
   std::istringstream(line) >> num;
   return num;
}

void Test() {
   using namespace std;
   istringstream iss("2\nthird_level.second_level.first_level\ndomain.ru\n5\nthird_level.second_level.first_level\ndomain.ru\n.sub.domain.ru\nsub.sub.domain.ru\nfourth_level.third_level.second_level.first_level\n");

   vector<Domain> v_forbidden = ReadDomains(iss, ReadNumberOnLine<int>(iss));
   assert(v_forbidden.at(0) == Domain("third_level.second_level.first_level"));
   assert(v_forbidden.at(1) == Domain("domain.ru"));

   Domain first_domain("aaa.aa");
   assert(first_domain == Domain("aaa.aa"));
   assert(first_domain < Domain("aaaa"));
   assert(first_domain.IsSubdomain(Domain("aaaa.aaa.aa")));
   assert(!first_domain.IsSubdomain(Domain("aa")));

   Domain second_domain("aaa.a.a.a.aa");
   assert(second_domain.IsSubdomain(Domain("a.aaa.aaa.a.a.a.aa")));

   vector<Domain> v_check = ReadDomains(iss, ReadNumberOnLine<size_t>(iss));
   DomainChecker checker_v(v_forbidden.begin(), v_forbidden.end());
   for (const auto& item : v_check) {
      assert(checker_v.IsForbidden(item));
   }
   assert(!checker_v.IsForbidden(Domain("com")));
   assert(!checker_v.IsForbidden(Domain("first_level")));

   set<Domain> s_forbidden;
   for (const auto& item : v_forbidden) {
      s_forbidden.insert(item);
   }

   DomainChecker checker_s(s_forbidden.begin(), s_forbidden.end());
   for (const auto& item : v_check) {
      assert(checker_s.IsForbidden(item));
   }
   assert(!checker_s.IsForbidden(Domain("com")));
   assert(!checker_s.IsForbidden(Domain("first_level")));

   cout << "Test completed successfully" << endl;
}

int main() {
   using namespace std;
   Test();
   istringstream iss(R"(4
gdz.ru
maps.me
m.gdz.ru
com
7
gdz.ru
gdz.com
m.maps.me
alg.m.gdz.ru
maps.com
maps.ru
gdz.ua)");
   const std::vector<Domain> forbidden_domains = ReadDomains(iss, ReadNumberOnLine<size_t>(iss));
   DomainChecker checker_v(forbidden_domains.begin(), forbidden_domains.end());

   const std::vector<Domain> test_domains = ReadDomains(iss, ReadNumberOnLine<size_t>(iss));
   for (const Domain& domain : test_domains) {
      cout << (checker_v.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
   }
}