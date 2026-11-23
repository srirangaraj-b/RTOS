# RTOS  - Real Time Operating Systems

## Operating Systems:
* An Operating system (OS) is a system software that manages computer hardware and provides services to application programs

	For Eg:
	* Windows 
	* Linux 
	* Mac
* It acts as an intermediary between hardware and software

	For Eg:
	* CPU
	* RAM
	* Storage
	* Keyboard
	* Display
	* Network Interface 


## RTOS: 
* An RTOS is an operating system designed to execute task within predictable timing constraints.
* Unlike Regular OS, RTOS's primary goal is predictable and bounded response time.

## Real-Time System vs General-Purpose System

|Feature|General-purpose OS|RTOS|
|---|---|---|
|Primary objective|Overall system performance|Timing predictability|
|Scheduling|General workload optimization|Real-time requirements|
|Response time|May vary significantly|Designed to be bounded|
|Task priority|Usually supported|Central to scheduling|
|Determinism|Not always guaranteed|Important design objective|
|Typical application|Desktop computers|Embedded control systems|

## Types of Real Time Systems:
* Hard Real-Time System
* Firm Real-Time System
* Soft Real-Time System

| Type | Deadline behavior           | Consequence                |
| ---- | --------------------------- | -------------------------- |
| Hard | Must not be missed          | Failure                    |
| Firm | Late result is useless      | Quality or functional loss |
| Soft | Occasional misses tolerated | Reduced performance        |

### Determinism:
Determinism means that the system behaves predictably under defined conditions.

For an RTOS, determinism particularly concerns execution and response times,

Eg:
Suppose a task needs 2 ms of the CPU execution time.
* Best-case execution time: 2ms
* Worst-case execution time: 2ms

However, the actual response time may be longer because the task could be waiting for CPU access, synchronization, or an interrupt.

A deterministic system aims to establish meaningful upper bounds or these delays.

Two important terms:
* WCET: Worst-Case Execution Time.
* BCET: Best-Case Execution Time.

Determinism is one of the most important principles in real-time system design.

# RTOS  Architecture:

RTOS generally consist of several major components.

|Component|Responsibility|
|---|---|
|Kernel|Core operating system functionality|
|Scheduler|Determines which task executes|
|Task management|Creates and manages tasks|
|Interrupt management|Handles hardware interrupts|
|Synchronization|Coordinates shared resources|
|IPC|Communication between tasks|
|Memory management|Manages memory resources|
|Timer management|Handles timing events|
|Device drivers|Interfaces with hardware|

## 1. Kernel:

The kernel is the central component of an operating system.

It manages the fundamental operations required to execute multiple tasks.

Its responsibilities include:
* Task Management
* Scheduling
* Context Switching
* Synchronization
* Inter-Task Communication
* Timing Services

An RTOS kernel is generally much smaller than a desktop operating system kernel.

However, the exact functionality depends on RTOS implementation.

For example:

* Zephyr has a modular kernel with a broad set of operating system services.
* FreeRTOS provides a relatively small real-time kernel.
* ThreadX provides real-time scheduling and synchronization services.
