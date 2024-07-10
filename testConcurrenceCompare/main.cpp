#include <iostream>
#include <vector>
#include <tuple>
#include <chrono>

using namespace std;

bool IsPrime(int n) {
  if (n <= 1) return false;
  for (int i = 2; i * i <= n; ++i) {
    if (n % i == 0) return false;
  }
  return true;
}

tuple<int, int> LoadInput() {
  int low, high;
  cout << "Please enter two numbers: ";
  cin >> low >> high;
  return make_tuple(low, high);
}

int main() {
  auto [low, high] = LoadInput();
  long long sum = 0;

  auto start = chrono::system_clock::now();

  for (int i = low; i <= high; ++i) {
    if (IsPrime(i)) {
      sum += i;
    }
  }

  auto end = chrono::system_clock::now();
  auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

  cout << "Total sum of primes between " << low << " and " << high << ": " << sum << endl;
  cout << "Time spent: " << static_cast<double>(duration.count()) / 1000.0 << " milliseconds" << endl;

  return 0;
}
