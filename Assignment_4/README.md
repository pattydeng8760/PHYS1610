# Friction Module and Test Model

## Overview
This project simulates the motion of a marble falling through a viscous liquid and collects data at regular time intervals. The goal is to estimate the **friction rate ($\alpha$)** using the recorded trajectory data.

The project consists of:
- A **friction module** that calculates the friction rate from velocity data.
- A **test model** that generates synthetic trajectory data.
- A **data analysis pipeline** that estimates the friction rate based on the collected data.

## Physical Model
The marble's velocity and position evolve according to the following equations:

$$v(t) = (v_0 + \frac{g}{\alpha}) e^{-\alpha t} - \frac{g}{\alpha}$$

$
z(t) = z_0 + \frac{1}{\alpha} (v_0 + \frac{g}{\alpha}) (1 - e^{-\alpha t}) - \frac{g}{\alpha} t
$

where:
- $ z_0 $ is the initial height (m),
- $ v_0 $ is the initial velocity (m/s),
- $ g $ is gravitational acceleration (m/s²),
- $ \alpha $ is the friction rate (1/s).

To estimate $ \alpha $, we compute the numerical derivative of the recorded position data to obtain velocity values, then use an averaging method based on acceleration ratios:

$
\alpha = \frac{1}{\Delta t} \ln \left[ \frac{1}{n - 2} \sum_{i=1}^{n-2} \frac{v_{i+1} - v_i}{v_{i+2} - v_{i+1}} \right]
$

## Code Structure
The project consists of the following components:

### **1. Friction Module (`friction.h`, `friction.cpp`)**
- `double frictionrate(double dt, const rvector<double>& v);`  
  Computes the friction rate given a set of velocity samples.
  
- `rvector<double> numdiff(double dt, const rvector<double>& z);`  
  Numerically differentiates position data to estimate velocities.

### **2. Test Model (`testmodel.cpp`)**
This program generates synthetic trajectory data based on the physical model. The data is stored in a file and can be used for analysis.

### **3. Model Implementation (`model.h`, `model.cpp`)**
Contains the equations that describe the marble's motion.

### **4. Data Analysis (`analyze.cpp`)**
Performs numerical differentiation and estimates the friction rate.

## How to Build and Run
### **Compiling the Code**
To compile the project, simply run:

make all
make analysis