### A Pluto.jl notebook ###
# v0.14.7

using Markdown
using InteractiveUtils

# ╔═╡ 2af93869-3776-42b6-9c29-567a95eebada
begin
	import Pkg
	
	Pkg.add("CSV")
	Pkg.add("DataFrames")
	Pkg.add("Plots")
	Pkg.add("StatsPlots")
end

# ╔═╡ 65ad7096-2c35-4458-a1a4-1d1394db9561
using CSV

# ╔═╡ 938108a5-4960-4d56-9ac1-5923129828e0
using DataFrames

# ╔═╡ 0edae56c-9987-4e45-8ff0-1160c853e90d
using Plots

# ╔═╡ c5980f00-7668-45f1-a0f7-c8c8babdef4b
using StatsPlots

# ╔═╡ e316f241-8a09-445b-bab9-0550ae1ef265
begin
	df = Array{DataFrame}(undef,2)
	df[1] = DataFrames.DataFrame(CSV.File("./src/data/mergesort/Intel(R) Core(TM) i5-7400 CPU @ 3.00GHz.csv"))
	df[2] = DataFrames.DataFrame(CSV.File("./src/data/mergesort/AMD FX(tm)-8350 Eight-Core Processor.csv"))
end

# ╔═╡ af8c6b6d-207f-467e-b5f2-7e1c3339c135
function GenericPlot(dfs::Array{DataFrame}, xlabel::String, ylabel::String, label::Array{String})
	p = plot(dfs[1][!, xlabel], dfs[1][!, ylabel], xlabel=xlabel, ylabel=ylabel, label = label[1]) 
	
	for i = 2:length(dfs)
		plot!(dfs[i][!, xlabel], dfs[i][!, ylabel], xlabel=xlabel, ylabel=ylabel, label = label[i])
	end
	
	
	p
end

# ╔═╡ abbb58de-3cfa-43bf-8ce5-c0f372ea4787
GenericPlot(df, "Length", "Elapsed Time", ["i5-7400", "FX-8350"])

# ╔═╡ 987eaaa2-af8a-470b-b512-95949022ddb7
GenericPlot(df, "Length", "L1 Cache miss", ["i5-7400", "FX-8350"])

# ╔═╡ 8c30c109-d0a9-4a2e-a65a-6dd5b2abc5cb
GenericPlot(df, "Length", "L2 Cache miss", ["i5-7400", "FX-8350"])

# ╔═╡ b47c0602-81b2-408b-b873-810ec17f25ea
GenericPlot(df, "Length", "Num of instructions", ["i5-7400", "FX-8350"])

# ╔═╡ Cell order:
# ╠═2af93869-3776-42b6-9c29-567a95eebada
# ╠═65ad7096-2c35-4458-a1a4-1d1394db9561
# ╠═938108a5-4960-4d56-9ac1-5923129828e0
# ╠═0edae56c-9987-4e45-8ff0-1160c853e90d
# ╠═c5980f00-7668-45f1-a0f7-c8c8babdef4b
# ╠═e316f241-8a09-445b-bab9-0550ae1ef265
# ╠═af8c6b6d-207f-467e-b5f2-7e1c3339c135
# ╠═abbb58de-3cfa-43bf-8ce5-c0f372ea4787
# ╠═987eaaa2-af8a-470b-b512-95949022ddb7
# ╠═8c30c109-d0a9-4a2e-a65a-6dd5b2abc5cb
# ╠═b47c0602-81b2-408b-b873-810ec17f25ea
