import {Injectable} from '@angular/core';
import {HttpClient} from '@angular/common/http';
import {Observable, timeout} from 'rxjs';
import {Status} from '../model/status.model';
import {DeviceInfo} from '../model/device-info';
import {Config} from '../model/config.model';

@Injectable()
export class DeviceService {

  constructor(private http: HttpClient) {}

  discover(): Observable<DeviceInfo> {
    return this.http.get<DeviceInfo>('/discovery');
  }

  getStatus(): Observable<Status> {
    return this.http.get<Status>('/monitor').pipe(
      timeout(1000)
    );
  }

  getConfig(): Observable<Config> {
    return this.http.get<Config>('/json');
  }

  updateConfig(config: Config): Observable<Config> {
    return this.http.post<Config>('/json', config);
  }

  reconnect(): Observable<null> {
    return this.http.get<null>('/reconnect');
  }

  reset(): Observable<null> {
    return this.http.get<null>('/reset');
  }

  defaults(): Observable<null> {
    return this.http.get<null>('/defaults');
  }
}
