import { spawn } from 'node:child_process';
import { createServer } from 'node:http';
import { readFile } from 'node:fs/promises';
import { extname, join } from 'node:path';
import { fileURLToPath } from 'node:url';
import { chromium } from 'playwright';
import { jest } from '@jest/globals';

jest.setTimeout(120000);

const rootDir = fileURLToPath(new URL('..', import.meta.url));
const webDir = join(rootDir, 'web');

const mimeTypes = {
  '.html': 'text/html',
  '.js': 'application/javascript',
  '.wasm': 'application/wasm',
  '.data': 'application/octet-stream',
  '.ogg': 'audio/ogg'
};

const run = (command, args, options = {}) =>
  new Promise((resolve, reject) => {
    const child = spawn(command, args, { stdio: 'inherit', ...options });
    child.on('exit', (code) => {
      if (code === 0) resolve();
      else reject(new Error(`${command} ${args.join(' ')} failed: ${code}`));
    });
  });

const startServer = () =>
  new Promise((resolve) => {
    const server = createServer(async (req, res) => {
      const urlPath = req.url === '/' ? '/index.html' : req.url;
      const filePath = join(webDir, decodeURIComponent(urlPath));
      try {
        const data = await readFile(filePath);
        const contentType = mimeTypes[extname(filePath)] || 'application/octet-stream';
        res.writeHead(200, { 'Content-Type': contentType });
        res.end(data);
      } catch {
        res.writeHead(404);
        res.end('Not found');
      }
    });

    server.listen(0, () => {
      const { port } = server.address();
      resolve({ server, port });
    });
  });

let server;
let port;
let browser;
let page;

beforeAll(async () => {
  await run('bash', ['scripts/nix-web-build.sh'], { cwd: rootDir });
  const serverInfo = await startServer();
  server = serverInfo.server;
  port = serverInfo.port;
  browser = await chromium.launch({
    args: [
      '--use-gl=swiftshader',
      '--enable-webgl',
      '--ignore-gpu-blocklist'
    ]
  });
  page = await browser.newPage({ viewport: { width: 1280, height: 720 } });
});

afterAll(async () => {
  if (browser) {
    await browser.close();
  }
  if (server) {
    server.close();
  }
});

test('canvas matches window size on load', async () => {
  await page.goto(`http://127.0.0.1:${port}/`, { waitUntil: 'domcontentloaded' });
  await page.waitForSelector('#canvas');
  await page.waitForFunction(() => {
    const canvas = document.getElementById('canvas');
    return canvas && canvas.width > 1 && canvas.height > 1;
  }, { timeout: 30000 });

  const result = await page.evaluate(() => {
    const canvas = document.getElementById('canvas');
    const width = window.innerWidth;
    const height = window.innerHeight;
    return {
      width,
      height,
      canvasWidth: canvas.width,
      canvasHeight: canvas.height,
      cssWidth: canvas.style.width,
      cssHeight: canvas.style.height
    };
  });

  const within = (actual, expected, tolerance) =>
    Math.abs(actual - expected) <= tolerance;

  if (!within(result.canvasWidth, result.width, 2) || !within(result.canvasHeight, result.height, 2)) {
    console.log('Canvas sizing mismatch', result);
  }

  expect(within(result.canvasWidth, result.width, 2)).toBe(true);
  expect(within(result.canvasHeight, result.height, 2)).toBe(true);
});
