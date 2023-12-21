import {Component, OnInit} from '@angular/core';
import {FormControl, FormGroup} from '@angular/forms';
import {DeviceService} from './service/device.service';
import {Config} from './model/config.model';
import {concatMap, delay, of, retry, tap, timer} from 'rxjs';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.scss']
})
export class AppComponent implements OnInit {

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

  configForm = new FormGroup({
    universe: new FormControl<number>(0),
    offset: new FormControl<number>(0),
    pixels: new FormControl<number>(0),
    leds: new FormControl<number>(0),
    white: new FormControl<number>(0),
    brightness: new FormControl<number>(0),
    hsv: new FormControl<number>(0),
    mode: new FormControl<number>(0),
    reverse: new FormControl<number>(0),
    speed: new FormControl<number>(0),
    split: new FormControl<number>(0),
  });

  constructor(private device: DeviceService) {}

  ngOnInit() {
    this.configForm.disable();
    this.device.getConfig().subscribe(config => {
      this.configForm.enable();
      this.configForm.patchValue(config);
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

  onReconnect(): void {
    if (confirm('Confirm network reconnect')) {
      this.device.reconnect().subscribe();
    }
  }

  onReboot(): void {
    if (confirm('Confirm device reboot')) {
      this.device.reset().subscribe();
    }
  }

  onReset(): void {
    if (confirm('Confirmt factory reset')) {
      this.device.defaults().subscribe();
    }
  }
}

