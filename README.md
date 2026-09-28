## Boid Simulation

A small project implementing boid simulation using [Raylib](https://www.raylib.com/)
Boids are capable of separation, alignment and grouping.

There is also a flow field algorithm. I use **inverse dijkstra** that create a cost grid and transforming it into
a flow field using a **Sobel Filter** that the boid can follow. I will be integrated into boids later.

For the boids i implemented some optimizations such as a Data Oriented Programming, grid partitioning and
multithreading.
I also added an option to display density at boid and grid level;

### Parameters :
- You can find weight for boid behavior in ``` BoidSim/BoidManager.h ``` alongside some other parameters
  such as range, perception angle, max speed and many others.
- You can find window size ```" InitWindow(1280, 720 ... " ```, boid number ``` " ... BoidManager(1000) " ```
  and terrain source ``` " ... Terrain(resources/breeze_1024.png) " ```  in ``` Engine.cpp ```.

### Further Upgrades
It is possible to optimize boids by switching to a **kdTree** instead of the grid or improving grid checking to test
neighbors from cells in front of the boid.
It is also possible to imply randomness or learning in each boids weights.

---

### Measured average boids update time
Update time are measured with 800 by 800 window, 1000 and 10000 boids. Time represent per boid update time.
Average on 1 minute of simulation. Optimizations are cumulative.

| Boid Optimizations       | 1k / 10k        |
|--------------------------|-----------------|
| Baseline (data-oriented) | 4.54µs / 46.8µs |
| Grid                     | 0.90µs / 6.96µs |
| Multithread              | 3.19µs / 1.26µs |

### Measured Dijkstra time
Dijkstra time is measured using ``` breeze_1024.png ``` and ``` breeze_2048.png ``` as map.
Averaging 100 iterations over random destinations. Every test is in release build.
Optimizations are cumulative.


| Dijkstra Optimizations                     | 1024x1024 | 2048x2048 |
|--------------------------------------------|-----------|-----------|
| Baseline (naive using priority_queue)      | 19.0ms    | 81.6ms    |
| Flattened 2D array (for cost grid and map) | 16.9ms    | 75.8ms    |
| Small changes                              | 16.4ms    | 74.5ms    |
