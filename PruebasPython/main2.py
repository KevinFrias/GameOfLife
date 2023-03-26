import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation


# Set grid size
n = 2500

# Initialize grid with random values
grid = np.random.choice([0, 1], size=(n, n))

# Define function to apply game rules to each cell in the grid
def apply_rules(grid):
    # Copy grid to create new generation
    new_grid = np.copy(grid)
    # Apply rules to each cell in the grid
    for i in range(n):
        for j in range(n):
            # Count live neighbors
            num_neighbors = np.sum(grid[max(0, i-1):min(n, i+2), max(0, j-1):min(n, j+2)]) - grid[i, j]
            # Apply rules based on number of neighbors
            if grid[i, j] == 1 and num_neighbors < 2:
                new_grid[i, j] = 0
            elif grid[i, j] == 1 and num_neighbors > 3:
                new_grid[i, j] = 0
            elif grid[i, j] == 0 and num_neighbors == 3:
                new_grid[i, j] = 1
    return new_grid

# Define function to animate the game
def animate_game():
    fig, ax = plt.subplots()
    img = ax.imshow(grid, cmap='binary')
    plt.axis('off')
    plt.title('Conway\'s Game of Life')
    # Define function to update the grid
    def update(frame):
        global grid
        grid = apply_rules(grid)
        img.set_data(grid)
        return img,
    # Animate the game
    anim = animation.FuncAnimation(fig, update, frames=100, interval=50, blit=True)
    plt.show()

# Call the animate_game function to start the game
animate_game()
