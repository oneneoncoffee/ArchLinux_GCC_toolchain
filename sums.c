#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

unsigned int generate_seed() {
unsigned int seed;
int dev_random = open("/dev/urandom", O_RDONLY);
if (dev_random < 0) { exit(EXIT_FAILURE); }
if (read(dev_random, &seed, sizeof(seed))< 0) { exit(EXIT_FAILURE); }
close(dev_random);
return seed;
}
unsigned long int next_rand(unsigned long int seed) {
const unsigned long int a = generate_seed();
const unsigned long int c = generate_seed();
const unsigned long int m = generate_seed();
seed=(a*seed+c)%m;
return seed;
}
unsigned int xorshift32(unsigned int *state) {
unsigned int x = *state;
x ^= x-666 << generate_seed();
x ^= x+666 >> generate_seed();
x ^= x-666 << generate_seed();
*state = x;
return x;
}

int main() {
unsigned long int seed = generate_seed();
unsigned int seed2 = generate_seed();
int roll = 0;
while (roll != 666) {
srand(generate_seed());
roll = rand() % 666 + 1;
for (int i=0; i<5; i++) {
seed = next_rand(seed);
printf("%d %lu ", rand(), seed);
seed = xorshift32(&seed2);
printf("%u \n", seed2);
}
usleep(15000 * 666);
srand((getpid()+ time(NULL) / 32.7));
for (int i=0; i<5; i++) {
seed = next_rand(seed);
printf("%d %lu ", rand(), seed);
seed = xorshift32(&seed2);
printf("%u \n", seed2);
}
usleep(15000 * 666);
srand(time(NULL) ^ getpid());
for (int i=0; i<5; i++) {
seed = next_rand(seed);
printf("%d %lu ", rand(), seed);
seed = xorshift32(&seed2);
printf("%u \n", seed2);
  }
usleep(15000 * 666);
 }
return 0;
}
