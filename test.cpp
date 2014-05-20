#include <threes_Mechanics.h>

int main() {
  Direction d = D;
  Direction u = U;

  Shift s0(d, 1);
  Shift s1(d, 0);
  std::vector<Shift> shifts;
  s0.string_vec.push_back(0);
  s0.string_vec.push_back(1);
  s0.string_vec.push_back(0);
  s0.string_vec.push_back(3);

  s1.string_vec.push_back(0);
  s1.string_vec.push_back(1);
  s1.string_vec.push_back(0);
  s1.string_vec.push_back(3);

  if (s0 < s1) {
    std::cout << "s0 < s1\n";
  } else {
    std::cout << "s0 > s1\n";
  }

  shifts.push_back(s0);
  shifts.push_back(s1);
  std::sort(shifts.begin(), shifts.end());

  for (Shift s : shifts) {
    std::cout << "id: " << s.id << ", str: ";
    for (int i = 0; i < 4; i++) {
      std::cout << s.string_vec[i] << " ";
    }
    std::cout << "\n";
  }
}