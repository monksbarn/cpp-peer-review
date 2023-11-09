#include <iostream>
#include <string>
#include <exception>
#include <vector>
#include <sstream>
#include <iomanip>

void PrintEBookStat(std::istream& in) {
   const size_t USERS_DB_SIZE = 1e+5;
   const size_t PAGES_FOR_USERS_SIZE = 1e+3;
   int requests_count, id, pages_count, users_count = 0;
   std::string key;
   std::vector<int> users_db(USERS_DB_SIZE);//индекс вектора - id, значение - количестов страниц (размер исходя из условия задачи 10^5)
   std::vector<int> pages_for_users(PAGES_FOR_USERS_SIZE);//индекс вектора - количество страниц, значение - количество пользователей с тем же значением страниц (размер исходя из условия 10^3)
   in >> requests_count;
   while (requests_count--) {
      in >> key >> id;
      if (key == "READ") {
         in >> pages_count;
         users_count += (users_db[id] == 0);//если users_db[id] == 0 истинно, то прибавляем единицу, т.к. это новый пользователь, в протином случае прибавляем ноль
         pages_for_users[users_db[id]] -= (users_db[id] != 0);//если users_db[id] != 0, то из вектора pages_for_users по индексу users_db[id] минусуем старые данные
         users_db[id] = pages_count;
         ++pages_for_users[pages_count];
      }
      else if (key == "CHEER") {
         if (users_count > 1) {
            double sum = 0;
            for (int i = 0; i < users_db[id]; ++i) {//считаем кол-во пользователей, количество прочитанных страниц у которых меньше, чем значение в users_db[id]
               sum += pages_for_users[i];
            }
            std::cout << std::setprecision(6) << sum / (users_count - 1) /* вычитаем единицу, чтобы исключить текущего пользователя в ключе CHEER */ << std::endl;
         }
         else {
            std::cout << (users_db[id] != 0) /* если пользователь есть, то выражение users_db[id] != 0 вернет 1 */ << std::endl;
         }
      }
      else {
         throw std::logic_error("Error in function " + std::string{ __func__ } + "(). Key \"" + key + "\" not exists");
      }
   }
}

int main() {
   std::istringstream iss(R"(18
CHEER 5
READ 1 10
CHEER 1
READ 2 5
READ 3 7
CHEER 2
CHEER 3
READ 3 10
CHEER 3
READ 3 11
CHEER 3
CHEER 1 
READ 1 20
CHEER 1
READ 2 15
CHEER 2
READ 4 17
CHEER 4)");
   PrintEBookStat(iss);
   return 0;
   /*
0
1
0
0.5
0.5
1
0.5
1
0.5
0.666667
    */
}
