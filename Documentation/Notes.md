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