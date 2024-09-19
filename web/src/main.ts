import ImageConverter from './image.converter';
import { decode } from 'upng-js'

export async function convert(jackTextAreaQuerySelector: string, binaryImageTextAreaQuerySelector: string, inputQuerySelector: string, exportSizeCheckboxQuerySelector: string, disableGroupingsCheckboxQuerySelector: string) {

  const isDev = import.meta.env.MODE === 'development'

  const target = document.querySelector(inputQuerySelector) as HTMLInputElement;
  const files = target.files;

  const jackTextArea = document.querySelector(jackTextAreaQuerySelector) as HTMLTextAreaElement;
  const errorOutputDiv = document.querySelector('#error-output') as HTMLDivElement;

  if (isDev && jackTextArea == null) {
    errorOutputDiv.textContent = "Can't find jack text area";
    errorOutputDiv.classList.remove('hidden');
    return;
  }

  const binaryImageTextArea = document.querySelector(binaryImageTextAreaQuerySelector) as HTMLTextAreaElement;
  if (isDev && binaryImageTextArea == null) {
    errorOutputDiv.textContent = "Can't find binary image text area";
    errorOutputDiv.classList.remove('hidden');
    return;
  }

  if (!files || files.length === 0) {
    errorOutputDiv.textContent = "No file selected";
    errorOutputDiv.classList.remove('hidden');
    return;
  }

  const file = files[0];
  const buffer = await file.arrayBuffer();
  const img = decode(buffer);
  const data = img.data;
  const exportSizeCheckbox = document.querySelector(exportSizeCheckboxQuerySelector) as HTMLInputElement;
  const exportSize = exportSizeCheckbox?.checked
  if (isDev && exportSizeCheckbox == null) {
    errorOutputDiv.textContent = "Can't find export size checkbox";
    errorOutputDiv.classList.remove('hidden');
    return;
  }
  const disableGroupingsCheckbox = document.querySelector(disableGroupingsCheckboxQuerySelector) as HTMLInputElement;
  if (isDev && disableGroupingsCheckbox == null) {
    errorOutputDiv.textContent = "Can't find disable groupings checkbox";
    errorOutputDiv.classList.remove('hidden');
    return;
  }
  const groupPixels = disableGroupingsCheckbox?.checked;
  if (isDev) {
    console.log("Export size:", exportSize);
    console.log("Group pixels :", groupPixels);
  }
  try {
    const { binaryImage, jackCode } = ImageConverter.convert(new Uint8Array(data), img.width, img.height, exportSize, groupPixels);
    jackTextArea.value = jackCode;
    binaryImageTextArea.value = binaryImage;
    errorOutputDiv.classList.add('hidden');
  } catch (e) {
    errorOutputDiv.textContent = (e as Error).message;
    errorOutputDiv.classList.remove('hidden');
  }

}

function copyToClipboard(elementId: string, copyButtonId: string) {
  const textarea = document.querySelector<HTMLTextAreaElement>(elementId);
  if (textarea) {
    textarea.select();
    navigator.clipboard.writeText(textarea.value);
    const button = document.querySelector<HTMLButtonElement>(copyButtonId)
    console.log(button)
    if (button) {
      button.textContent = "Copied!";
      setInterval(() => {
        button.textContent = "Copy";
      }, 700);
    } else {
      const errorOutputDiv = document.querySelector('#error-output') as HTMLDivElement;
      errorOutputDiv.textContent = "Can't find copy button";
      errorOutputDiv.classList.remove('hidden');
    }
  } else {
    const errorOutputDiv = document.querySelector('#error-output') as HTMLDivElement;
    errorOutputDiv.textContent = "Cannot copy text to clipboard. Element not found.";
    errorOutputDiv.classList.remove('hidden');
  }
}

const convertInvocation = () => convert("#output", '#binary_img', "#input", "#export_size", "#disable_grouping")

function reconvert() {
  //check if file has been selected
  if (!document.querySelector<HTMLInputElement>(filesInputQuerySelector)?.files?.length) {
    const errorOutputDiv = document.querySelector('#error-output') as HTMLDivElement;
    errorOutputDiv.textContent = "No file selected. Please select an image file.";
    errorOutputDiv.classList.remove('hidden');
    return;
  }
  convertInvocation();
}

const filesInputQuerySelector = '#input';

document.querySelector<HTMLButtonElement>(filesInputQuerySelector)?.addEventListener('change', convertInvocation);

document.querySelector<HTMLButtonElement>('#copy_button')?.addEventListener('click', () => copyToClipboard('#output', "#copy_button"));

document.querySelector<HTMLButtonElement>('#disable_grouping')?.addEventListener('click', () => reconvert());
document.querySelector<HTMLButtonElement>('#export_size')?.addEventListener('click', () => reconvert());