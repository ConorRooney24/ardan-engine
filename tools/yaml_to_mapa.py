# yaml_to_mapa
#
# takes the path of a .yaml map file, and creates a corresponding .mapa file in the same location.

from __future__ import annotations
import argparse
from pathlib import Path
import sys
import yaml

def load_yaml(path):

	with path.open("r", encoding="utf-8") as fh:

		try:
			data = yaml.safe_load(fh)

		except yaml.YAMLError as e:
			raise ValueError(f"Error: Unable to open yaml '{e}'") from e
		
	if not isinstance(data, dict):
		raise ValueError("Error: Invalid yaml structure")
	
	return data


def validate_yaml(data):

	if "name" not in data:
		raise ValueError("Error: Missing map name")
	
	name = str(data["name"])

	if "size" not in data or not isinstance(data["size"], dict):
		raise ValueError("Error: missing size data")
	
	try:
		size_x = int(data["size"]["x"])
		size_y = int(data["size"]["y"])
		size_z = int(data["size"]["z"])

	except Exception as e:
		raise ValueError("Error: sizes must be integers") from e
	
	if size_x <= 0 or size_y <= 0 or size_z <= 0:
		raise ValueError("Error: sizes must be positive integers")

	spawn = data.get("spawn", {})
	pos = spawn.get("position", {}) if isinstance(spawn, dict) else {}
	rot = spawn.get("rotation", {}) if isinstance(spawn, dict) else {}

	try:
		spawn_pos_x = float(pos.get("x", 0.0))
		spawn_pos_y = float(pos.get("y", 0.0))
		spawn_pos_z = float(pos.get("z", 0.0))
		spawn_rot_x = float(rot.get("x", 0.0))
		spawn_rot_y = float(rot.get("y", 0.0))

	except Exception as e:
		raise ValueError("Error: Invalid spawn point information given") from e

	# voxels
	if "voxels" not in data:
		raise ValueError("Error: Missing voxel information")
	
	voxels = data["voxels"]

	if not isinstance(voxels, list):
		raise ValueError("Error: Invalid voxel information given")

	expected_rows = size_y * size_z

	if len(voxels) != expected_rows:
		raise ValueError(f"Error: Invalid number of voxel rows given")

	normalized_rows = []

	for idx, row in enumerate(voxels):

		if not isinstance(row, list):
			raise ValueError(f"Error: Invalid voxel information given on row {idx}")
		
		if len(row) != size_x:
			raise ValueError(f"Error: Invalid number of columns given on row {idx}")
		
		try:
			normalized_rows.append([int(v) for v in row])

		except Exception as e:
			raise ValueError(f"Error: Invalid data given on row {idx}") from e

	return {"name": name, "size_x": size_x, "size_y": size_y, "size_z": size_z, "spawn_pos_x": spawn_pos_x, "spawn_pos_y": spawn_pos_y, "spawn_pos_z": spawn_pos_z, "spawn_rot_x": spawn_rot_x, "spawn_rot_y": spawn_rot_y, "voxels_rows": normalized_rows}


def build_grid(info):

	size_x = info["size_x"]
	size_y = info["size_y"]
	size_z = info["size_z"]
	rows = info["voxels_rows"]
	grid = [[[0 for _ in range(size_x)] for _ in range(size_z)] for _ in range(size_y)]
	rows_per_y = size_z

	for y_block in range(size_y):

		y = (size_y - 1) - y_block
		base = y_block * rows_per_y

		for row_in_block in range(rows_per_y):

			z = (size_z - 1) - row_in_block
			row = rows[base + row_in_block]

			for x in range(size_x):

				grid[y][z][x] = int(row[x])
	return grid


def write_mapa_file(out_path, info, grid):

	with out_path.open("w", encoding="utf-8") as fh:

		fh.write(f"{info['name']}\n")

		fh.write(f"{info['size_x']}\n")
		fh.write(f"{info['size_y']}\n")
		fh.write(f"{info['size_z']}\n")

		fh.write(f"{info['spawn_pos_x']}\n")
		fh.write(f"{info['spawn_pos_y']}\n")
		fh.write(f"{info['spawn_pos_z']}\n")

		fh.write(f"{info['spawn_rot_x']}\n")
		fh.write(f"{info['spawn_rot_y']}\n")

		fh.write("---\n")
		size_x = info['size_x']
		size_y = info['size_y']
		size_z = info['size_z']
		
		for y in range(size_y):

			for z in range(size_z):

				for x in range(size_x):

					fh.write(f"{x},{y},{z},{grid[y][z][x]}\n")


def main():

	argp = argparse.ArgumentParser(description=".yaml to .mapa converter")
	argp.add_argument("input", type=str, help="path to .yaml map file")
	argp.add_argument("-f", "--force", action="store_true", help="Overwrite existing .mapa file")
	args = argp.parse_args()


	in_path = Path(args.input)
	out_path = in_path.with_suffix(".mapa")


	if not in_path.is_file():
		print(f"ERROR: input not found: {in_path}", file=sys.stderr)
		sys.exit(2)


	if out_path.exists() and not args.force:
		print(f"ERROR: {out_path} already exists. Use --force to overwrite.", file=sys.stderr)
		sys.exit(3)

	try:
		raw = load_yaml(in_path)
		info = validate_yaml(raw)
		grid = build_grid(info)
		write_mapa_file(out_path, info, grid)

	except Exception as e:
		print(f"ERROR: {e}", file=sys.stderr)
		sys.exit(4)

	print(f"Wrote: {out_path}")



if __name__ == "__main__":
	main()