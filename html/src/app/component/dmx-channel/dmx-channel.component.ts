import {Component} from '@angular/core';
import {
  AbstractControl,
  ControlValueAccessor,
  FormControl,
  FormGroup,
  NG_VALIDATORS,
  NG_VALUE_ACCESSOR,
  ValidationErrors,
  Validator,
  Validators
} from '@angular/forms';
import {distinctUntilChanged, map} from 'rxjs';

@Component({
  selector: 'app-dmx-channel',
  templateUrl: './dmx-channel.component.html',
  styleUrls: ['./dmx-channel.component.scss'],
  providers: [
    {
      provide: NG_VALUE_ACCESSOR,
      multi: true,
      useExisting: DmxChannelComponent
    },
    {
      provide: NG_VALIDATORS,
      multi: true,
      useExisting: DmxChannelComponent
    }
  ]
})
export class DmxChannelComponent implements ControlValueAccessor, Validator {

  binary: number[] = [];
  form = new FormGroup({
    channel: new FormControl<number>(0, [
      Validators.required,
      Validators.min(0),
      Validators.max(511)
    ])
  });
  disabled = false;
  touched = false;

  constructor() {
    this.form.controls.channel.valueChanges.pipe(
      map((value) => {
        if (!this.isNumber(value) || value < 0) {
          value = 0;
        } else if (value > 511) {
          value = 511;
        }
        this.form.controls.channel.setValue(value, {emitEvent: false});

        return value;
      }),
      distinctUntilChanged(),
    ).subscribe(value => {
      this.onChange(value);

      this.binary = ('000000000' + (value >>> 0).toString(2))
        .slice(-9)
        .split('')
        .reverse()
        .map(state => +state)
    });
  }

  onChange = (_: number) => {};

  onTouched = () => {};

  onDecrease(): void {
    this.markAsTouched();
    if (!this.disabled) {
      this.form.setValue({channel: this.getChangedValue(-1)});
    }
  }

  onIncrease(): void {
    this.markAsTouched();
    if (!this.disabled) {
      this.form.setValue({channel: this.getChangedValue(1)});
    }
  }

  onDipSwitch(index: number) {
    if (!this.disabled && index >= 0 && index < 9 && this.binary?.length === 9) {
      this.binary[index] = this.binary[index] === 1 ? 0 : 1;
      this.form.setValue({channel: parseInt(this.binary.reverse().join(''), 2)});
    }
  }

  writeValue(channel: number) {
    this.form.setValue({channel});
  }

  registerOnChange(onChange: any) {
    this.onChange = onChange;
  }

  registerOnTouched(onTouched: any) {
    this.onTouched = onTouched;
  }

  markAsTouched() {
    if (!this.touched) {
      this.onTouched();
      this.touched = true;
    }
  }

  setDisabledState(disabled: boolean) {
    this.disabled = disabled;
    if (disabled) {
      this.form.disable();
    } else {
      this.form.enable();
    }
  }

  validate(_: AbstractControl): ValidationErrors | null {
    return this.form.get('channel')?.errors || null;
  }

  private isNumber(value: number | null): value is number {
    return value !== null && !isNaN(value);
  }

  private getChangedValue(change: number): number {
    const channel = this.form.controls.channel.value;
    return this.isNumber(channel) ? channel + change : 0;
  }
}
