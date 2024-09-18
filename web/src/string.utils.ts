export class StringUtils {
    static split(str: string, delimiter: string): string[] {
        const result: string[] = [];
        const lines = str.split(delimiter);
        for (const line of lines) {
            result.push(line);
        }
        return result;
    }

    static stripMargin(s: string): string {
        const lines = this.split(s, '\n');
        let res = '';
        for (let line of lines) {
            let delimiterPos = 0;
            for (let i = 0; i < line.length; ++i) {
                const c = line[i];
                if (c === ' ' || c === '\t') {
                    continue;
                } else if (c === '|') {
                    delimiterPos = i + 1;
                    break;
                } else {
                    break;
                }
            }
            res += line.substring(delimiterPos) + "\n";
        }
        return res;
    }

    static toString(pixels: boolean[][]): string {
        let res="";
        for (const pixel of pixels) {
            for (const j of pixel) {
                res+= j ? "1" : "0";
            }
            res+="\n";
        }
        return res;
    }
}

