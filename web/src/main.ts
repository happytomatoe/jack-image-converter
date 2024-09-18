import ImageConverter from './image.converter';
import './style.css'
import { decode } from 'upng-js'
export function setupCounter(element: HTMLButtonElement, jackTextArea: HTMLButtonElement, binaryImageTextArea: HTMLButtonElement) {
  const onChange = async (e: Event) => {
    const target = e.target as HTMLInputElement | HTMLTextAreaElement;

    if (target instanceof HTMLInputElement && target.type === 'file') {
      const files = target.files;
      console.log("Files");

      if (files) {

        const file = files[0];
        const buffer = await file.arrayBuffer();
        const img = decode(buffer);
        const data = img.data;
        console.log("Decoded");
        const { binaryImage, jackCode } = ImageConverter.convert(new Uint8Array(data), img.width, img.height, false, false,
        );
        jackTextArea.value = jackCode;
        console.log("Binary img", binaryImage)
        binaryImageTextArea.value = binaryImage;

      }
    }
  };

  element.addEventListener('change', onChange);
}

setupCounter(document.querySelector<HTMLButtonElement>('#input')!, document.querySelector<HTMLButtonElement>('#output')!,
  document.querySelector<HTMLButtonElement>('#binary_img')!
);

