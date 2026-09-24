## Boid Simulation

A small project implementing boid simulation using [Raylib](https://www.raylib.com/)
Boid are capable of separation, alignement and grouping.

I implemented some optimizations such as a Data oriented management and a grid partitionning.

You can find weight for boid behavior in [``` BoidSim/BoidManager.h ```](https://github.com/JoaBories/BoidSimulation/blob/main/sources/BoidSim/BoidManager.h) alongside some other parameters such as range, perception angle and max speed.
You can find ```GRID_SIZE``` parameter in [``` BoidSim/BoidGrid.h ```](https://github.com/JoaBories/BoidSimulation/blob/main/sources/BoidSim/BoidGrid.h)
Finally you can find window size ```" InitWindow(1280, 720 ... " ``` and boid number ``` " ... BoidManager(1000) " ``` in [``` Engine.cpp ```](https://github.com/JoaBories/BoidSimulation/blob/main/sources/Engine.cpp)

### Further Upgrades
It is possible to optimize by switching to a **kdTree** instead of the grid or improving grid checking to test neighbors from cells in front of the boid.

## Mesured average boids update time
Update time are mesured with 800 by 800 window, 500 boids and are Whole update time and per boid update time. Average on 1 minute of simulation.
Optimizations are cumulative.

| Optimizations                 | Debug           | Release         |
|-------------------------------|-----------------|-----------------|
| Baseline (data-oriented)      | 5.74ms / 11.4µs | 1.15ms / 2.29µs |
| Grid (5x5 cells)              | 2.56ms / 5.12µs | 0.44ms / 0.80µs |
| Grid (10x10 cells)            | 1.39ms / 2.77µs | 0.24ms / 0.49µs |
| Use of noexcept and constexpr | 1.34ms / 2.68µs | 0.21ms / 0.42µs |