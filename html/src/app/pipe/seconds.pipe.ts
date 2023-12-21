import {Pipe, PipeTransform} from "@angular/core";

@Pipe({
  standalone: true,
  name: 'seconds'
})
export class SecondsPipe implements PipeTransform {

  transform(value: string | number, exponent = 1): string {
    if (isNaN(+value)) {
      return value.toString();
    }
    const date = new Date(0);
    date.setSeconds(+value);
    return date.toISOString().substring(11, 19);
  }
}
