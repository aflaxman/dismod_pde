#! /bin/bash -e
python_dir="../python"
symbol_list='o*x+pdh^>v<'
alpha='1'
# age_list="10 20 30 40 50 60 70 80 90"
age_list="10 30 50 70 90"
# cohort_list="1860 1880 1900 1920 1940 1960 1980 2000 2020 2040"
cohort_list="1860 1900 1940 1980 2020"
#
$python_dir/d4_plot.py \
	compare_sfun.csv screen 'Gaussian Simulation (line is truth)' \
	--line cohort solution $symbol_list $alpha \
	--points cohort estimate "$symbol_list" $alpha \
	--string function iota \
	--axis cohort 1820 2020 iota .5e-4 2.5e-4 \
	--connect age $age_list 
#
$python_dir/d4_plot.py \
	compare_sfun.csv screen 'Gaussian Simulation (line is truth)' \
	--line age solution $symbol_list $alpha \
	--points age estimate "$symbol_list" $alpha \
	--string function iota \
	--axis age 0 100  iota .5e-4 2.5e-4 \
	--connect cohort $cohort_list
#
$python_dir/d4_plot.py \
	sfun_out.csv screen 'Gaussian Simulation Residuals' \
	--points cohort age_residual "$symbol_list" $alpha \
	--string function iota \
	--numeric age_residual 'age_residual != inf' \
	--axis cohort 1820 2020 'iota residual' -1e-5 1e-5 \
	--connect age $age_list 
#
$python_dir/d4_plot.py \
	sfun_out.csv screen 'Gaussian Simulation Residuals' \
	--points age  age_residual "$symbol_list" $alpha \
	--string function iota \
	--numeric age_residual 'age_residual != inf' \
	--axis age 0 100 'iota residual' -1e-5 1e-5 \
	--connect cohort $cohort_list 
