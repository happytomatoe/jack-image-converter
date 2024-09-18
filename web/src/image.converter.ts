import { StringUtils } from './string.utils';
import { Rectangle } from './rectangle';
export interface Result {
    binaryImage: string;
    jackCode: string;
}
class ImageConverter {
    static convert(image: Uint8Array, w: number, h: number, exportSize: boolean, disableGrouping: boolean,): Result {
        const img = this.convertToBinaryImage(image, w, h);
        const binaryImage = StringUtils.toString(img)
        const jackCode = this.convertToJackCode(img, exportSize, disableGrouping)
        return { binaryImage, jackCode };
    }

    static convertToJackCode(img: boolean[][], exportSize: boolean, disableGrouping: boolean): string {
        let header = '';
        if (exportSize) {
            header = `
                |field int imageW, imageH;
                |let imageW = ${img[0].length};
                |let imageH = ${img.length};
            `;
        }
        header += `method void draw(int x, int y) {
        `;
        let res = header;
        if (disableGrouping) {
            for (let i = 0; i < img.length; i++) {
                const row = img[i];
                for (let j = 0; j < row.length; j++) {
                    if (!row[j]) {
                        const jStr = j > 0 ? `+${j}` : '';
                        const iStr = i > 0 ? `+${i}` : '';
                        res += `|       do Screen.drawPixel(x${jStr},y${iStr});\n`;
                    }
                }
            }
        } else {
            for (const r of this.findRectangles(img)) {
                res += r.convertToJackCode();
            }
        }
        const footer = `|    return;
            |}
        `;
        return StringUtils.stripMargin(res + footer);
    }

    static convertToBinaryImage(image: Uint8Array, w: number, h: number): boolean[][] {
        if (w <= 0 || h <= 0) {
            throw new Error(`Invalid image size: ${w}x${h}`);
        }
        const res: boolean[][] = Array(h).fill(null).map(() => Array(w).fill(false));
        for (let y = 0; y < h; y++) {
            for (let x = 0; x < w * 4; x += 4) {
                const n = image[y * w * 4 + x];
                if (n > 224) res[y][x / 4] = true;
            }
        }
        return res;
    }

    static findRectangles(image: boolean[][]): Rectangle[] {
        const rows = image.length;
        const cols = image[0].length;
        const visited: boolean[][] = Array(rows).fill(null).map(() => Array(cols).fill(false));
        const rectangles: Rectangle[] = [];

        for (let i = 0; i < rows; ++i) {
            for (let j = 0; j < cols; ++j) {
                if (!image[i][j] && !visited[i][j]) {
                    // Start a new rectangle
                    let minX = j, minY = i;
                    let maxX = j, maxY = i;

                    // Expand to the right
                    while (maxX + 1 < cols && !image[minY][maxX + 1] && !visited[minY][maxX + 1]) {
                        ++maxX;
                    }

                    // Expand downward
                    let canExpandDown = true;
                    while (canExpandDown && maxY + 1 < rows) {
                        for (let x = minX; x <= maxX; ++x) {
                            if (image[maxY + 1][x] || visited[maxY + 1][x]) {
                                canExpandDown = false;
                                break;
                            }
                        }
                        if (canExpandDown) {
                            ++maxY;
                        }
                    }

                    // Mark the rectangle as visited
                    for (let y = minY; y <= maxY; ++y) {
                        for (let x = minX; x <= maxX; ++x) {
                            visited[y][x] = true;
                        }
                    }

                    // Store the rectangle
                    rectangles.push(new Rectangle(minX, minY, maxX, maxY));
                }
            }
        }

        return rectangles;
    }
}

export default ImageConverter;

