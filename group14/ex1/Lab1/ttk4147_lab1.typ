#set text(font: "New Computer Modern Math")

#show heading.where(level: 1): it => {
  set text(size: 25pt)
  set align(center)
  set block(below: 2em)
  it
} 

#show heading.where(level: 2): it => {
  set text(size: 18pt) 
  set block(below: 1.5em)
  it
}

#show heading.where(level: 3): it => {
  set text(size: 16pt)
  it
}

#set text(size: 14pt)

= TTK4147 - Lab 0
#v(20pt)



== TASK A
#v(10pt)

*For* ```c sleep(1) ```: \

real  0m1.005s \
user  0m0.001s \
sys   0m0.004s \
#v(20pt)

*For*  ```c busy_wait(1)``` with  ```c clock_gettime(CLOCK_MONOTONIC, ...) ```: \

real  0m1.004s \
user  0m1.003s \
sys   0m0.001s \
#v(20pt)

*For* ```c busy_wait(1)``` with  ```c times()```: \
real  0m0.004s \
user  0m0.001s \
sys   0m0.004s \
#v(20pt)

```c busy_wait(1)``` with ```c times()``` og ```c sleep(1)``` bruker (ca) same sys tid. \
```c times()``` opererer hovudsakelig i kernel space.

#v(250pt)




== TASK B
Estimert *access latency* for ```c _rdtsc()```: \
$"real" 0m 0.111s$ / $(10*1000*1000)$ \

$= 1.11*10^(-8)$s \
#v(20pt)

*Resolution*:
#figure(
  image("plot_rdtsc.png", width: 80%)
)

#v(40pt)


Estimert *access latency* for ```c clock_gettime(CLOCK_MONOTONIC, &now)```: \
$"real" 0m 0.359s$ / $(10*1000*1000)$ \

$= 3.59*10^(-8)$s \
#v(300pt)

*Resolution*:
#figure(
  image("plot_clock.png", width: 80%)
)
#v(40pt)


Estimert *access latency* for ```c times()```: \
$"real" 0m 10.467s$ / $(10*1000*1000)$ \

$= 0.0010467s$


*Resolution*
#figure(
  image("plot_times.png", width: 80%)
)
