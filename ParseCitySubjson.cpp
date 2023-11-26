#include <string>
#include <vector>
#include <map>

using namespace std;

enum class Language;

struct City;

class Json {
public:
   vector<Json> AsList() const;
   map<string, Json>& AsObject() const;
   string AsString() const;
};

struct Country {
   string name;
   string iso_code;
   string phone_code;
   string time_zone;
   vector<Language> languages;
   vector<City> cities;
};

struct City {
   string country_name;
   string country_iso_code;
   string phone_code;
   string city_name;
   string city_iso_code;
   string city_time_zone;
   vector<Language> languages;
};

// Дана функция ParseCitySubjson, обрабатывающая JSON-объект со списком городов конкретной страны:
void ParseCitySubjson(vector<City>& cities, const Json& json, const Country& country) {
   for (const auto& city_json : json.AsList()) {
      auto& city_obj = city_json.AsObject();
      cities.push_back({
         city_obj["name"s].AsString(),
         city_obj["iso_code"s].AsString(),
         country.phone_code + city_obj["phone_code"s].AsString(),
         country.name,
         country.iso_code,
         country.time_zone,
         country.languages });
   }
}

template<typename Language>
Language FromString(const string& lang);

// ParseCitySubjson вызывается только из функции ParseCountryJson следующим образом:
void ParseCountryJson(vector<Country>& countries, vector<City>& cities, const Json& json) {
   for (const auto& country_json : json.AsList()) {
      auto& country_obj = country_json.AsObject();
      countries.push_back({
          country_obj["name"s].AsString(),
          country_obj["iso_code"s].AsString(),
          country_obj["phone_code"s].AsString(),
          country_obj["time_zone"s].AsString(),
         });
      Country& country = countries.back();
      for (const auto& lang_obj : country_obj["languages"s].AsList()) {
         country.languages.push_back(FromString<Language>(lang_obj.AsString()));
      }
      ParseCitySubjson(cities, country_obj["cities"], country);
   }
}