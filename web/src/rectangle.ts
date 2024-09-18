export class Rectangle {
    top_left_x: number;
    top_left_y: number;
    bottom_right_x: number;
    bottom_right_y: number;

    // Constructor to initialize the rectangle's corners
    constructor(min_x: number, min_y: number, max_x: number, max_y: number) {
        this.top_left_x = min_x;
        this.top_left_y = min_y;
        this.bottom_right_x = max_x;
        this.bottom_right_y = max_y;
    }

    // Overload the equality operator to compare two rectangles
    equals(other: Rectangle): boolean {
        return this.top_left_x === other.top_left_x &&
               this.top_left_y === other.top_left_y &&
               this.bottom_right_x === other.bottom_right_x &&
               this.bottom_right_y === other.bottom_right_y;
    }

    // Method to calculate the size (area) of the rectangle
    size(): number {
        const width = this.bottom_right_x - this.top_left_x + 1;
        const height = this.bottom_right_y - this.top_left_y + 1;
        return width * height;
    }

    toString(): string {
        return `Rectangle(${this.top_left_x}, ${this.top_left_y}, ${this.bottom_right_x}, ${this.bottom_right_y})`;
    }

    isLine(): boolean {
        return (this.top_left_x === this.bottom_right_x || this.top_left_y === this.bottom_right_y) &&
               this.size() > 1;
    }

    convertToJackCode(): string {
        let x0 = this.top_left_x !== 0 ? `+${this.top_left_x}` : '';
        let y0 = this.top_left_y !== 0 ? `+${this.top_left_y}` : '';
        let x1 = this.bottom_right_x !== 0 ? `+${this.bottom_right_x}` : '';
        let y1 = this.bottom_right_y !== 0 ? `+${this.bottom_right_y}` : '';

        if (this.size() === 1) return `|    do Screen.drawPixel(x${x0},y${y0});\n`;
        const func_name = this.isLine() ? "Screen.drawLine" : "Screen.drawRectangle";
        return `|    do ${func_name}(x${x0},y${y0},x${x1},y${y1});\n`;
    }
}

