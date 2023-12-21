import {Component, OnInit} from '@angular/core';
import {FormControl, FormGroup, Validators} from '@angular/forms';
import {DeviceService} from './service/device.service';
import {Config} from './model/config.model';
import {ColorMapping, colorMapping} from './type/color-mapping';
import {ArtnetConfig} from './model/artnet-config.model';
import {concatMap, delay, of, retry, tap, timer} from 'rxjs';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.scss']
})
export class AppComponent implements OnInit {

  protected readonly colorMapping = colorMapping;

  healthy = false;

  deviceStatus$ = timer(0, 1000).pipe(
    concatMap(() => this.device.getStatus()),
    retry({count: Infinity, delay: () => {
      this.healthy = false;
      return of(null).pipe(delay(5000));
    }}),
  ).pipe(
    tap(() => {
      this.healthy = true;
    })
  );

  deviceInfo$ = this.device.discover();

  modeList = Array(16)
    .fill(0)
    .map((mode, i) => ({id: i, description: 'TODO'}))
    .map(mode => ({value: mode.id, name: `Mode ${mode.id}`}));

  modeForm = new FormGroup({
    mode: new FormControl<number>(0, [Validators.required, Validators.min(0), Validators.max(15)]),
  });

  artnetForm = new FormGroup({
    universe: new FormControl<number>(0, [Validators.required]),
    offset: new FormControl<number>(0, [Validators.required]),
  });

  configForm = new FormGroup({
    pixels: new FormControl<number>(0, [Validators.required, Validators.min(0)]),
    colorMapping: new FormControl<ColorMapping>('RGB', Validators.required),
    white: new FormControl<boolean>(false, Validators.required),
    speed: new FormControl<number>(0, Validators.required),
    split: new FormControl<number>(0, Validators.required),
    reverse: new FormControl<boolean>(false, Validators.required),
    hsv: new FormControl<boolean>(false, Validators.required),
    brightness: new FormControl<number>(255, [Validators.required, Validators.min(0), Validators.max(255)]),
  });

  constructor(private device: DeviceService) {}

  ngOnInit() {
    this.configForm.disable();
    this.device.getConfig().subscribe(config => {
      this.configForm.enable();
      this.configForm.patchValue(config);
    });

    this.modeForm.disable();
    this.device.getMode().subscribe(mode => {
      this.modeForm.enable();
      this.modeForm.patchValue(mode);
      this.modeForm.controls.mode.valueChanges.subscribe((mode) => {
        if (mode !== null && !isNaN(mode) && this.modeForm.valid) {
          this.modeForm.disable({emitEvent: false});
          this.device.setMode({mode}).subscribe(mode => {
            this.modeForm.patchValue(mode, {emitEvent: false});
            this.modeForm.enable({emitEvent: false});
          });
        }
      });
    });

    this.artnetForm.disable();
    this.device.getChannel().subscribe(artnetConfig => {
      this.artnetForm.enable();
      this.artnetForm.patchValue(artnetConfig);
    });

    // Accordion behavior
    const detailsElements = document.querySelectorAll('details');
    detailsElements.forEach(element => {
      element.addEventListener('toggle', () => {
        if (element.open) {
          detailsElements.forEach(target => {
            if (target !== element && target.open) {
              target.open = false;
            }
          });
        }
      });
    });
  }

  onSubmitConfig(): void {
    if (!confirm('Confirm configuration update')) {
      return;
    }
    this.configForm.disable();
    this.device.updateConfig(this.configForm.value as Config).subscribe(config => {
      this.configForm.patchValue(config);
      this.configForm.enable();
    });
  }

  onSubmitChannel(): void {
    if (!confirm('Confirm channel switch')) {
      return;
    }
    this.artnetForm.disable();
    this.device.setChannel(this.artnetForm.value as ArtnetConfig).subscribe((config) => {
      this.artnetForm.patchValue(config);
      this.artnetForm.enable();
    });
  }

  onReconnect(): void {
    if (confirm('Confirm network reconnect')) {
      this.device.reconnect().subscribe();
    }
  }

  onReboot(): void {
    if (confirm('Confirm device reboot')) {
      this.device.restart().subscribe();
    }
  }

  onReset(): void {
    if (confirm('Confirmt factory reset')) {
      this.device.reset().subscribe();
    }
  }
}

