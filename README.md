## Boid Simulation

A small project implementing boid simulation using [Raylib](https://www.raylib.com/)
Boid are capable of separation, alignement and grouping. I Added a flow field implementation to make boid 
follow an objective. I use **inverse dijkstra** that create a cost grid and transforming it into 
a flow field using a **Sobel Filter** that the boid can follow.

For the boids i implemented some optimizations such as a Data oriented management and a grid partitionning.

### Parameters :
- You can find weight for boid behavior in [``` BoidSim/BoidManager.h ```](https://github.com/JoaBories/BoidSimulation/blob/main/sources/BoidSim/BoidManager.h) alongside some other parameters 
such as range, perception angle and max speed. 
- You can find ```GRID_SIZE``` parameter in [``` BoidSim/BoidGrid.h ```](https://github.com/JoaBories/BoidSimulation/blob/main/sources/BoidSim/BoidGrid.h). 
- Finally you can find window size ```" InitWindow(1280, 720 ... " ``` and boid number ``` " ... BoidManager(1000) " ``` 
in [``` Engine.cpp ```](https://github.com/JoaBories/BoidSimulation/blob/main/sources/Engine.cpp)

### Further Upgrades
It is possible to optimize boids by switching to a **kdTree** instead of the grid or improving grid checking to test 
neighbors from cells in front of the boid.
It is also possible to imply randomness or learning in each boids weights.

---

### Mesured average boids update time
Update time are mesured with 800 by 800 window, 500 boids and are Whole update time and per boid update time. Average on 1 minute of simulation.
Optimizations are cumulative.

| Boid Optimizations            | Debug           | Release         |
|-------------------------------|-----------------|-----------------|
| Baseline (data-oriented)      | 5.74ms / 11.4µs | 1.15ms / 2.29µs |
| Grid (5x5 cells)              | 2.56ms / 5.12µs | 0.44ms / 0.80µs |
| Grid (10x10 cells)            | 1.39ms / 2.77µs | 0.24ms / 0.49µs |
| Use of noexcept and constexpr | 1.34ms / 2.68µs | 0.21ms / 0.42µs |

### Mesured Dijkstra time
Dijkstra time is mesured using ``` breeze_1024.png ``` and ``` breeze_2048.png ``` as map.
Averaging 100 iterations over random destinations. Every test is in release build.
Optimizations are cumulative.

| Dijkstra Optimizations                     | 1024x1024 | 2048x2048 |
|--------------------------------------------|-----------|-----------|
| Baseline (naive using priority_queue)      | 19.0ms    | 81.6ms    |
| Flattened 2D array (for cost grid and map) | 16.9ms    | 75.8ms    |
| Small changes                              | 16.4ms    | 74.5ms    |