import numpy as np
import tkinter as tk

GRID_SIZE = 175
UPDATE_INTERVAL_MS = 20

new_matrix = [[0 for j in range(5000)] for i in range(5000)]

def update_grid(grid):
    new_grid = np.zeros_like(grid)
    for i in range(len(grid)):
        for j in range(len(grid[0])):
            num_neighbors = (
                grid[i - 1][j - 1]
                + grid[i - 1][j]
                + grid[i - 1][(j + 1) % len(grid[0])]
                + grid[i][(j + 1) % len(grid[0])]
                + grid[(i + 1) % len(grid)][(j + 1) % len(grid[0])]
                + grid[(i + 1) % len(grid)][j]
                + grid[(i + 1) % len(grid)][j - 1]
                + grid[i][(j - 1) % len(grid[0])]
            )
            if grid[i][j] == 1 and (num_neighbors == 2 or num_neighbors == 3):
                new_grid[i][j] = 1
            elif grid[i][j] == 0 and num_neighbors == 3:
                new_grid[i][j] = 1
    return new_grid

def update_ui(grid, canvas):
    new_grid = update_grid(grid)
    draw_grid(new_grid, canvas)
    canvas.after(UPDATE_INTERVAL_MS, update_ui, new_grid, canvas)

def draw_grid(grid, canvas):
    canvas_width = canvas.winfo_width()
    canvas_height = canvas.winfo_height()
    canvas.delete("rect")
    cell_width = canvas_width // len(grid[0])
    cell_height = canvas_height // len(grid)
    for i in range(len(grid)):
        for j in range(len(grid[0])):
            x0 = j * cell_width
            y0 = i * cell_height
            x1 = (j + 1) * cell_width
            y1 = (i + 1) * cell_height
            if grid[i][j]:
                canvas.create_rectangle(x0, y0, x1, y1, fill="black", tags="rect")
            else:
                canvas.create_rectangle(x0, y0, x1, y1, fill="white", tags="rect")

root = tk.Tk()
root.title("Conway's Game of Life")

canvas = tk.Canvas(root, width=1400, height=700, bg="white")
canvas.pack(fill=tk.BOTH, expand=True)

grid = np.random.choice([0, 1], size=(GRID_SIZE, GRID_SIZE))

draw_grid(grid, canvas)
canvas.after(UPDATE_INTERVAL_MS, update_ui, grid, canvas)

root.mainloop()
