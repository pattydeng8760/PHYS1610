#!/bin/bash
#SBATCH --ntasks=1
#SBATCH --time=01:00:00

module load gcc rarray boost 
module list 
echo "C++ Modules loaded"

# The soruce of the code directory
source_dir="/home/l/lcl_uotphy1610/lcl_uotphy1610s1050/PHYS_1060/Assignment_4/"

OUTPUT=output-$TEMP-$SLURM_JOB_ID
mkdir -p $OUTPUT
NUMSEEDS=10  # Number of simulations

# Define parameter ranges
Z_START=0.0   # Min initial position
Z_END=5.0     # Max initial position
V_START=5.0   # Min initial velocity
V_END=15.0    # Max initial velocity
T_START=10.0  # Min time duration
T_END=20.0    # Max time duration
D_START=0.01   # Min time step
D_END=0.5     # Max time step

# Systematically vary parameters across NUMSEEDS
for (( SEED=1; SEED<=NUMSEEDS; SEED++ )); do
    echo "Running simulation $SEED of $NUMSEEDS"

    # Systematic variation of parameters
    A=0.125
    G=9.8
    Z=0.120
    V=$(echo "$V_START + ($V_END - $V_START) * ($SEED - 1) / ($NUMSEEDS - 1)" | bc -l)
    T=$(echo "$T_START + ($T_END - $T_START) * ($SEED - 1) / ($NUMSEEDS - 1)" | bc -l)
    D=$(echo "$D_START + ($D_END - $D_START) * ($SEED - 1) / ($NUMSEEDS - 1)" | bc -l)

    # Format values for filenames (replace '.' with '_')
    A_F=$(printf "%.3f" "$A" | tr '.' '_')
    G_F=$(printf "%.3f" "$G" | tr '.' '_')
    Z_F=$(printf "%.3f" "$Z" | tr '.' '_')
    V_F=$(printf "%.3f" "$V" | tr '.' '_')
    T_F=$(printf "%.3f" "$T" | tr '.' '_')
    D_F=$(printf "%.3f" "$D" | tr '.' '_')

    # Define output filenames with formatted parameter values
    DATAFILE="$OUTPUT/testmodel_a${A_F}_g${G_F}_z${Z_F}_v${V_F}_t${T_F}_d${D_F}.dat"
    ANALYSISFILE="$OUTPUT/analysis_a${A_F}_g${G_F}_z${Z_F}_v${V_F}_t${T_F}_d${D_F}.out"

    # Run test model with varying parameters
    "./testmodel" -f "$DATAFILE" -a "$A" -g "$G" -z "$Z" -v "$V" -t "$T" -d "$D"

    # Run analysis on generated data
    "./analyze" -f "$DATAFILE" -o "$ANALYSISFILE"

    echo "Completed analysis for seed $SEED with parameters: a=$A_F, g=$G_F, z=$Z_F, v=$V_F, t=$T_F, d=$D_F"
done

echo "All simulations completed. Results stored in $OUTPUT/"