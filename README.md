 *Zadatak*. U repozitoriju se nalazi program za rješavanje linearne skalarne eliptičke
 jednadžbe 2. reda. Jednadžba je zadana u datoteci operator.hh.  


 Potrebno je generalizirati zadaću do zadaće oblika

```
   - div( a(x)grad u) + b(x) u = f(x)   u \Omega
                    u = g(x)   na \Gamma_D\subseteq\partial\Omega
        - a(x)grad u . n = j (x)  na \Gamma_N = \partial\Omega\setminus\Gamma_D
```

Odabrati netrivijalne funkcije q,b,f,g i j te naći egzaktno rješenje.
Rubni uvjet treba biti mješovitog tipa.
Izračunati L^2 normu greške.
Primjer mora raditi u 1, 2 i 3 prostorne dimenzije.
