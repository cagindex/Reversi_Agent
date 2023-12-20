# class Core::Checkboard

checkboard object

## Member functions

---

### void show(bool t) const

show the board in terminal. show available positions if $t$ is true, else not.

### void Input(int row, int col)

Put the chessman in ($row$, $col$).

### bool which_one_turn() const

Return whose turn now. ture for black, false for white.

### bool game_status() const

Return game status. true for game is running, false for game over.

### Core::AvaPos get_available() const

get the available positions.

- return type: {{ava_pos, targetpos1, targetpos2, ...}, ...}

### Core::data statustic() const

return the game status.

[Core::data](./structs.md#Core::data)

### char& at(int row, int col)

Get epecific position elements. Range row, col $[1, 8]$.

