can I bound the size of the stones?

multiplication by 2024 is multiplication by 8*253

multiplication by 2024 increases the number of digits by about 3.3
even numbers are divided into 2.

suppose that there is a number x. the number of digits is

D = CEIL[ LOG(x)/LOG(10) ]

the parity of the digits is given by 

P = D % 2
 = CEIL[ ( LOG(x)/LOG(10) ) % 2]

so we want to know how many reals there are such that
adding LOG(20.24) to them keeps them in the right range

so initially, 1 < x < 2.Therefore
0.3 < x + y < 1.4
of this set, the troublesome bits are when

1 < x + y  < 1.4

If this holds, then

0.3 < x + 2y < 0.8

So we are guaranteed that we will halve the number of digits whenever the number of digits is multiplied twice by 2024.

So if X begins with D digits, then it can achieve at most D + 7 digits before crashing being halved. Long term, D can't be more than 8 digits long
So there are at most
100_000_000
values for the stone if you start with integers less than that length

If you calculate the evolution of stones (starting with a base table of 1),
It will only take ITER*100_000_000 (7.5 BILLION) to reach the desired quantity.
How sure am I of the 100_000_000 bound?

If you're 8 digits long, then you're getting halved immediately.

Can we construct this dynamically (recursion)?

a b c d

a in map?
