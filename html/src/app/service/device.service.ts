import {Injectable} from '@angular/core';
import {HttpClient} from '@angular/common/http';
import {Observable, timeout} from 'rxjs';
import {Status} from '../model/status.model';
import {DeviceInfo} from '../model/device-info';
import {Config} from '../model/config.model';
import {ArtnetConfig} from '../model/artnet-config.model';
import {Mode} from '../model/mode.model';

@Injectable()
export class DeviceService {

  constructor(private http: HttpClient) {}

  discover(): Observable<DeviceInfo> {
    return this.http.get<DeviceInfo>('/discovery');
  }

  getStatus(): Observable<Status> {
    return this.http.get<Status>('/status').pipe(
      timeout(1000)
    );
  }

  getMode(): Observable<Mode> {
    return this.http.get<Mode>('/mode');
  }

  getChannel(): Observable<ArtnetConfig> {
    return this.http.get<ArtnetConfig>('/channel');
  }

  getConfig(): Observable<Config> {
    return this.http.get<Config>('/config');
  }

  setMode(mode: Mode): Observable<Mode> {
    return this.http.post<Mode>('/mode', mode);
  }

  setChannel(config: ArtnetConfig): Observable<ArtnetConfig> {
    return this.http.post<ArtnetConfig>('/channel', config);
  }

  updateConfig(config: Config): Observable<Config> {
    return this.http.post<Config>('/config', config);
  }

  reconnect(): Observable<null> {
    return this.http.get<null>('/reconnect');
  }

  restart(): Observable<null> {
    return this.http.get<null>('/reset');
  }

  reset(): Observable<null> {
    return this.http.get<null>('/defaults');
  }
}
